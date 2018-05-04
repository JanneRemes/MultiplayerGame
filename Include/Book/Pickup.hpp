#ifndef BOOK_PICKUP_HPP
#define BOOK_PICKUP_HPP

#include <Book/Entity.hpp>
#include <Book/Command.hpp>
#include <Book/ResourceIdentifiers.hpp>

#include <SFML/Graphics/Sprite.hpp>


class PlayerBat;

class Pickup : public Entity
{
	public:
		enum Type
		{
			Ball,
			HealthRefill,
			TypeCount
		};


	public:
								Pickup(Type type, const TextureHolder& textures);

		virtual unsigned int	getCategory() const;
		virtual sf::FloatRect	getBoundingRect() const;

		void 					apply(PlayerBat& player) const;

		void setWorldBounds(sf::FloatRect);


	protected:
		virtual void			drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
		virtual void			updateCurrent(sf::Time dt, CommandQueue& commands);

	private:
		Type 					mType;
		sf::Sprite				mSprite;

		sf::FloatRect worldBounds;
};

#endif // BOOK_PICKUP_HPP