#ifndef BALL_HPP
#define BALL_HPP

#include <Book/Entity.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML\Graphics\Texture.hpp>

class Ball : public Entity
{
public:
	Ball();
	~Ball();

	virtual sf::FloatRect	getBoundingRect() const;
	virtual void			updateCurrent(sf::Time dt, CommandQueue& commands);

	sf::Sprite				mSprite;
	sf::Texture				mTexture;
private:
	sf::Vector2f			mTargetDirection;
};

#endif