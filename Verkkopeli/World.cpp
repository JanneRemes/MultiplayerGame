#include <Book/World.hpp>
#include <Book/Projectile.hpp>
#include <Book/Pickup.hpp>
#include <Book/Foreach.hpp>
#include <Book/TextNode.hpp>
#include <Book/SoundNode.hpp>
#include <Book/NetworkNode.hpp>
#include <Book/Utility.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include <algorithm>
#include <cmath>
#include <limits>


World::World(sf::RenderTarget& outputTarget, FontHolder& fonts, SoundPlayer& sounds, bool networked)
	: mTarget(outputTarget)
	, mSceneTexture()
	, mWorldView(outputTarget.getDefaultView())
	, mTextures()
	, mFonts(fonts)
	, mSounds(sounds)
	, mSceneGraph()
	, mSceneLayers()
	, mWorldBounds(0.f, 0.f, mWorldView.getSize().x, mWorldView.getSize().y)
	, mSpawnPosition(mWorldView.getSize().x / 2.f, mWorldBounds.height - mWorldView.getSize().y / 2.f)
	, mPlayerBats()
	, mEnemySpawnPoints()
	, mActiveEnemies()
	, mNetworkedWorld(networked)
	, mNetworkNode(nullptr)
	, mFinishSprite(nullptr)
{
	mSceneTexture.create(mTarget.getSize().x, mTarget.getSize().y);

	loadTextures();
	buildScene();

	// Prepare the view
	mWorldView.setCenter(mSpawnPosition);
}

void World::update(sf::Time dt)
{
	/*FOREACH(PlayerBat* a, mPlayerBats)
		a->setVelocity(0.f, 0.f);*/

	// Setup commands to destroy entities
	destroyEntitiesOutsideView();

	// Forward commands to scene graph, adapt velocity (diagonal correction)
	while (!mCommandQueue.isEmpty())
		mSceneGraph.onCommand(mCommandQueue.pop(), dt);

	adaptPlayerVelocity();

	// Collision detection and response (may destroy entities)
	handleCollisions();

	// Remove PlayerBats that were destroyed (World::removeWrecks() only destroys the entities, not the pointers in mPlayerBat)
	auto firstToRemove = std::remove_if(mPlayerBats.begin(), mPlayerBats.end(), std::mem_fn(&PlayerBat::isMarkedForRemoval));
	mPlayerBats.erase(firstToRemove, mPlayerBats.end());

	// Remove all destroyed entities
	mSceneGraph.removeWrecks();

	// Regular update step, adapt position (correct if outside view)
	mSceneGraph.update(dt, mCommandQueue);
	adaptPlayerPosition();

	updateSounds();
}

void World::draw()
{
	mTarget.setView(mWorldView);
	mTarget.draw(mSceneGraph);
}

CommandQueue& World::getCommandQueue()
{
	return mCommandQueue;
}

PlayerBat* World::getPlayerBat(int identifier) const
{
	FOREACH(PlayerBat* playerBat, mPlayerBats)
	{
		if (playerBat->getIdentifier() == identifier)
			return playerBat;
	}

	return nullptr;
}

void World::removePlayerBat(int identifier)
{
	PlayerBat* PlayerBat = getPlayerBat(identifier);
	if (PlayerBat)
	{
		PlayerBat->destroy();
		mPlayerBats.erase(std::find(mPlayerBats.begin(), mPlayerBats.end(), PlayerBat));
	}
}

PlayerBat* World::addPlayerBat(int identifier)
{
	if (mPlayerBats.size() == 0)
	{
		std::unique_ptr<PlayerBat> player(new PlayerBat(PlayerBat::Player1, mTextures, mFonts));
		sf::Vector2f vec;
		vec.x = 0.50 * mWorldBounds.width;
		vec.y = 0.80 * mWorldBounds.height;
		player->setPosition(vec);
		player->setIdentifier(identifier);

		mPlayerBats.push_back(player.get());
		mSceneLayers[UpperAir]->attachChild(std::move(player));
	}
	else
	{
		std::unique_ptr<PlayerBat> player(new PlayerBat(PlayerBat::Player2, mTextures, mFonts));
		sf::Vector2f vec;
		vec.x = 0.50 * mWorldBounds.width;
		vec.y = 0.20 * mWorldBounds.height;
		player->setPosition(vec);
		player->setIdentifier(identifier);

		mPlayerBats.push_back(player.get());
		mSceneLayers[UpperAir]->attachChild(std::move(player));
	}

	return mPlayerBats.back();
}

void World::createPickup(sf::Vector2f position, Pickup::Type type)
{
	std::unique_ptr<Pickup> pickup(new Pickup(Pickup::Ball, mTextures));
	pickup->setPosition(position);
	pickup->setVelocity(0.f, 10.f);
	mSceneLayers[UpperAir]->attachChild(std::move(pickup));
}

bool World::pollGameAction(GameActions::Action& out)
{
	return mNetworkNode->pollGameAction(out);
}

void World::setCurrentBattleFieldPosition(float lineY)
{
	mWorldView.setCenter(mWorldView.getCenter().x, lineY - mWorldView.getSize().y / 2);
	mSpawnPosition.y = mWorldBounds.height;
}

void World::setWorldHeight(float height)
{
	mWorldBounds.height = height;
}

bool World::hasAlivePlayer() const
{
	return mPlayerBats.size() > 0;
}

bool World::hasPlayerReachedEnd() const
{
	if (PlayerBat* PlayerBat = getPlayerBat(1))
		return !mWorldBounds.contains(PlayerBat->getPosition());
	else
		return false;
}

void World::loadTextures()
{
	mTextures.load(Textures::Explosion, "Media/Textures/Explosion.png");
	mTextures.load(Textures::FinishLine, "Media/Textures/FinishLine.png");
	mTextures.load(Textures::Player1, "Media/Textures/player1.png");
	mTextures.load(Textures::Player2, "Media/Textures/player2.png");
	mTextures.load(Textures::Ball, "Media/Textures/ball.png");
}

void World::adaptPlayerPosition()
{
	// Keep player's position inside the screen bounds, at least borderDistance units from the border
	sf::FloatRect viewBounds = getViewBounds();
	const float borderDistance = 10.f;

	FOREACH(PlayerBat* PlayerBat, mPlayerBats)
	{
		sf::Vector2f position = PlayerBat->getPosition();
		position.x = std::max(position.x, viewBounds.left + borderDistance);
		position.x = std::min(position.x, viewBounds.left + viewBounds.width - borderDistance);
		position.y = std::max(position.y, viewBounds.top + borderDistance);
		position.y = std::min(position.y, viewBounds.top + viewBounds.height - borderDistance);
		PlayerBat->setPosition(position);
	}
}

void World::adaptPlayerVelocity()
{
	FOREACH(PlayerBat* playerBat, mPlayerBats)
	{
		sf::Vector2f velocity = playerBat->getVelocity();

		// If moving diagonally, reduce velocity (to have always same velocity)
		if (velocity.x != 0.f && velocity.y != 0.f)
			playerBat->setVelocity(velocity / std::sqrt(2.f));

		//playerBat->accelerate(0.f, -20.f); 
		//tässä kusee joku, also inputtien pitäisi myös accelerate
	}
}

bool matchesCategories(SceneNode::Pair& colliders, Category::Type type1, Category::Type type2)
{
	unsigned int category1 = colliders.first->getCategory();
	unsigned int category2 = colliders.second->getCategory();

	// Make sure first pair entry has category type1 and second has type2
	if (type1 & category1 && type2 & category2)
	{
		return true;
	}
	else if (type1 & category2 && type2 & category1)
	{
		std::swap(colliders.first, colliders.second);
		return true;
	}
	else
	{
		return false;
	}
}

void World::handleCollisions()
{
	std::set<SceneNode::Pair> collisionPairs;
	mSceneGraph.checkSceneCollision(mSceneGraph, collisionPairs);

	FOREACH(SceneNode::Pair pair, collisionPairs)
	{
		if (matchesCategories(pair, Category::PlayerBat, Category::EnemyBat))
		{
			auto& player = static_cast<PlayerBat&>(*pair.first);
			auto& enemy = static_cast<PlayerBat&>(*pair.second);

			// Collision: Player damage = enemy's remaining HP
			player.damage(enemy.getHitpoints());
			enemy.destroy();
		}

		else if (matchesCategories(pair, Category::PlayerBat, Category::Pickup))
		{
			/*auto& player = static_cast<PlayerBat&>(*pair.first);
			auto& pickup = static_cast<Pickup&>(*pair.second);

			// Apply pickup effect to player, destroy projectile
			pickup.apply(player);
			pickup.destroy();
			player.playLocalSound(mCommandQueue, SoundEffect::CollectPickup);*/
			auto& ball = static_cast<Pickup&>(*pair.second);

			ball.setVelocity(ball.getVelocity().x, ball.getVelocity().y * -1);
		}
		else if (matchesCategories(pair, Category::Goal, Category::Pickup))
		{
			// TODO
		}
	}
}

void World::updateSounds()
{
	sf::Vector2f listenerPosition;

	// 0 players (multiplayer mode, until server is connected) -> view center
	if (mPlayerBats.empty())
	{
		listenerPosition = mWorldView.getCenter();
	}

	// 1 or more players -> mean position between all PlayerBats
	else
	{
		FOREACH(PlayerBat* PlayerBat, mPlayerBats)
			listenerPosition += PlayerBat->getWorldPosition();

		listenerPosition /= static_cast<float>(mPlayerBats.size());
	}

	// Set listener's position
	mSounds.setListenerPosition(listenerPosition);

	// Remove unused sounds
	mSounds.removeStoppedSounds();
}

void World::buildScene()
{
	// Initialize the different layers
	for (std::size_t i = 0; i < LayerCount; ++i)
	{
		Category::Type category = (i == LowerAir) ? Category::SceneAirLayer : Category::None;

		SceneNode::Ptr layer(new SceneNode(category));
		mSceneLayers[i] = layer.get();

		mSceneGraph.attachChild(std::move(layer));
	}

	// Add the finish line to the scene
	/*sf::Texture& finishTexture = mTextures.get(Textures::FinishLine);
	std::unique_ptr<SpriteNode> finishSprite(new SpriteNode(finishTexture));
	finishSprite->setPosition(0.f, -76.f);
	mFinishSprite = finishSprite.get();
	mSceneLayers[Background]->attachChild(std::move(finishSprite));*/

	// Add the player1 goal to the scene
	Goal *goalP1 = new Goal(0, 5, 422, 730, mFonts);
	std::unique_ptr<SpriteNode> goalSprite(new SpriteNode(goalP1->textureGoal));
	goalSprite->setPosition(goalP1->posX, goalP1->posY);
	mGoalSprite = goalSprite.get();
	mSceneLayers[Background]->attachChild(std::move(goalSprite));
	// Add the player2 goal to the scene
	Goal *goalP2 = new Goal(1, 5, 422, 0, mFonts);
	std::unique_ptr<SpriteNode> goalSprite2(new SpriteNode(goalP2->textureGoal));
	goalSprite2->setPosition(goalP2->posX, goalP2->posY);
	mGoalSprite2 = goalSprite2.get();
	mSceneLayers[Background]->attachChild(std::move(goalSprite2));

	// Add sound effect node
	std::unique_ptr<SoundNode> soundNode(new SoundNode(mSounds));
	mSceneGraph.attachChild(std::move(soundNode));

	// Add network node, if necessary
	if (mNetworkedWorld)
	{
		std::unique_ptr<NetworkNode> networkNode(new NetworkNode());
		mNetworkNode = networkNode.get();
		mSceneGraph.attachChild(std::move(networkNode));
	}
}

void World::destroyEntitiesOutsideView()
{
	Command command;
	command.category = Category::Projectile | Category::EnemyBat;
	command.action = derivedAction<Entity>([this](Entity& e, sf::Time)
	{
		sf::FloatRect fr = getBattlefieldBounds();
		sf::FloatRect br = e.getBoundingRect();

		if (!fr.intersects(br))
			e.remove();
	});

	mCommandQueue.push(command);
}

/*void World::addGoals()
{
mGoals.push_back(std::shared_ptr<Goal>(new Goal(0, 5, 362, 0, mFonts)));
mGoals.push_back(std::shared_ptr<Goal>(new Goal(1, 5, 362, 730, mFonts)));
}*/

sf::FloatRect World::getViewBounds() const
{
	//return sf::FloatRect(sf::Vector2f(0, mWorldView.getSize().y), mWorldView.getSize());
	return sf::FloatRect(mWorldView.getCenter() - mWorldView.getSize() / 2.f, mWorldView.getSize());
}

sf::FloatRect World::getBattlefieldBounds() const
{
	// Return view bounds + some area at top, where enemies spawn
	sf::FloatRect bounds = getViewBounds();
	bounds.top -= 20.f;
	bounds.height += 20.f;

	return bounds;
}