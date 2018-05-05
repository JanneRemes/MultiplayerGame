#ifndef BOOK_PlayerGoal_HPP
#define BOOK_PlayerGoal_HPP

#include <Book/Entity.hpp>
#include <Book/Command.hpp>
#include <Book/ResourceIdentifiers.hpp>
#include <Book/Projectile.hpp>
#include <Book/TextNode.hpp>
#include <Book/Animation.hpp>

#include <SFML/Graphics/Sprite.hpp>


class PlayerGoal : public Entity
{
public:
	enum Type
	{
		Goal1,
		Goal2,
		TypeCount
	};


public:
	PlayerGoal(Type type, const TextureHolder& textures, const FontHolder& fonts);

	virtual unsigned int	getCategory() const;
	virtual sf::FloatRect	getBoundingRect() const;
	virtual void			remove();
	virtual bool 			isMarkedForRemoval() const;
	bool					isAllied() const;
	//float					getMaxSpeed() const;

	void					playLocalSound(CommandQueue& commands, SoundEffect::ID effect);
	int						getIdentifier();
	void					setIdentifier(int identifier);

	void setShowExplosion(bool);
	Type getType() { return mType; }

private:
	virtual void			drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual void 			updateCurrent(sf::Time dt, CommandQueue& commands);

	//void updateMovement(sf::Time);

	void					updateTexts();

private:
	Type					mType;
	sf::Sprite				mSprite;
	Animation				mExplosion;
	bool 					mShowExplosion;
	bool					mExplosionBegan;

	float					mTravelledDistance;
	std::size_t				mDirectionIndex;
	TextNode*				mHealthDisplay;

	int						mIdentifier;
};

#endif // BOOK_PlayerGoal_HPP
