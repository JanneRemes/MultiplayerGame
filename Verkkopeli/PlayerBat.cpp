#include <Book/PlayerBat.hpp>
#include <Book/DataTables.hpp>
#include <Book/Utility.hpp>
#include <Book/CommandQueue.hpp>
#include <Book/SoundNode.hpp>
#include <Book/NetworkNode.hpp>
#include <Book/ResourceHolder.hpp>

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

#include <cmath>


using namespace std::placeholders;

namespace
{
	const std::vector<PlayerBatData> Table = initializePlayerBatData();
}

PlayerBat::PlayerBat(Type type, const TextureHolder& textures, const FontHolder& fonts)
: Entity(Table[type].hitpoints)
, mType(type)
, mSprite(textures.get(Table[type].texture), Table[type].textureRect)
, mExplosion(textures.get(Textures::Explosion))
, mShowExplosion(true)
, mExplosionBegan(false)
, mDirectionIndex(0)
, mIdentifier(0)
{
	mExplosion.setFrameSize(sf::Vector2i(256, 256));
	mExplosion.setNumFrames(16);
	mExplosion.setDuration(sf::seconds(1));

	centerOrigin(mSprite);
	centerOrigin(mExplosion);

	std::unique_ptr<TextNode> healthDisplay(new TextNode(fonts, ""));
	mHealthDisplay = healthDisplay.get();
	attachChild(std::move(healthDisplay));

	updateTexts();
}

void PlayerBat::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (isDestroyed() && mShowExplosion)
		target.draw(mExplosion, states);
	else
			target.draw(mSprite, states);
}

void PlayerBat::updateCurrent(sf::Time dt, CommandQueue& commands)
{
	// Update texts and roll animation
	updateTexts();

	// Entity has been destroyed: Possibly drop pickup, mark for removal
	if (isDestroyed())
	{
		mExplosion.update(dt);

		// Play explosion sound only once
		if (!mExplosionBegan)
		{
			// Play sound effect
			SoundEffect::ID soundEffect = (randomInt(2) == 0) ? SoundEffect::Explosion1 : SoundEffect::Explosion2;
			playLocalSound(commands, soundEffect);

			// Emit network game action for enemy explosions
			if (!isAllied())
			{
				sf::Vector2f position = getWorldPosition();

				Command command;
				command.category = Category::Network;
				command.action = derivedAction<NetworkNode>([position] (NetworkNode& node, sf::Time)
				{
					node.notifyGameAction(GameActions::EnemyExplode, position);
				});

				commands.push(command);
			}

			mExplosionBegan = true;
		}
		return;
	}

	updateMovement(dt);
	Entity::updateCurrent(dt, commands);
}

void PlayerBat::updateMovement(sf::Time dt)
{
	const std::vector<Direction>& directions = Table[mType].directions;
	if (!directions.empty())
	{
		// Compute velocity from direction
		float radians = toRadian(directions[mDirectionIndex].angle + 90.f);
		float vx = getMaxSpeed() * std::cos(radians);
		float vy = getMaxSpeed() * std::sin(radians);

		setVelocity(vx, vy);

		mTravelledDistance += getMaxSpeed() * dt.asSeconds();
	}
}

unsigned int PlayerBat::getCategory() const
{
	if (isAllied())
		return Category::PlayerBat;
	else
		return Category::EnemyBat;
}

sf::FloatRect PlayerBat::getBoundingRect() const
{
	return getWorldTransform().transformRect(mSprite.getGlobalBounds());
}

bool PlayerBat::isMarkedForRemoval() const
{
	return isDestroyed() && (mExplosion.isFinished() || !mShowExplosion);
}

void PlayerBat::remove()
{
	Entity::remove();
	mShowExplosion = false;
}

bool PlayerBat::isAllied() const
{
	return mType == Player1;
}

float PlayerBat::getMaxSpeed() const
{
	return Table[mType].speed;
}

void PlayerBat::playLocalSound(CommandQueue& commands, SoundEffect::ID effect)
{
	sf::Vector2f worldPosition = getWorldPosition();

	Command command;
	command.category = Category::SoundEffect;
	command.action = derivedAction<SoundNode>(
		[effect, worldPosition] (SoundNode& node, sf::Time)
		{
			node.playSound(effect, worldPosition); 
		});

	commands.push(command);
}

int	PlayerBat::getIdentifier()
{
	return mIdentifier;
}

void PlayerBat::setIdentifier(int identifier)
{
	mIdentifier = identifier;
}

void PlayerBat::updateTexts()
{
	// Display hitpoints
	/*if (isDestroyed())
		mHealthDisplay->setString("");
	else
		mHealthDisplay->setString(toString(getHitpoints()) + " HP");
	mHealthDisplay->setPosition(0.f, 50.f);
	mHealthDisplay->setRotation(-getRotation());*/
}