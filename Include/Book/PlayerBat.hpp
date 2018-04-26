#ifndef BOOK_PlayerBat_HPP
#define BOOK_PlayerBat_HPP

#include <Book/Entity.hpp>
#include <Book/Command.hpp>
#include <Book/ResourceIdentifiers.hpp>
#include <Book/Projectile.hpp>
#include <Book/TextNode.hpp>
#include <Book/Animation.hpp>

#include <SFML/Graphics/Sprite.hpp>


class PlayerBat : public Entity
{
	public:
		enum Type
		{
			Eagle,
			Player1,
			Player2,
			TypeCount
		};


	public:
								PlayerBat(Type type, const TextureHolder& textures, const FontHolder& fonts);

		virtual unsigned int	getCategory() const;
		virtual sf::FloatRect	getBoundingRect() const;
		virtual void			remove();
		virtual bool 			isMarkedForRemoval() const;
		bool					isAllied() const;
		float					getMaxSpeed() const;
		void					disablePickups();

		void					playLocalSound(CommandQueue& commands, SoundEffect::ID effect);
		int						getIdentifier();
		void					setIdentifier(int identifier);

	private:
		virtual void			drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
		virtual void 			updateCurrent(sf::Time dt, CommandQueue& commands);
		void					checkPickupDrop(CommandQueue& commands);
		void					createPickup(SceneNode& node, const TextureHolder& textures) const;

		void					updateTexts();

	private:
		Type					mType;
		sf::Sprite				mSprite;
		Animation				mExplosion;
		bool 					mShowExplosion;
		bool					mExplosionBegan;
		bool					mSpawnedPickup;
		bool					mPickupsEnabled;

		Command 				mDropPickupCommand;
		float					mTravelledDistance;
		std::size_t				mDirectionIndex;
		TextNode*				mHealthDisplay;
	
		int						mIdentifier;};

#endif // BOOK_PlayerBat_HPP
