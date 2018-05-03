#include "Ball.h"

#include <iostream>

Ball::Ball() : 
	Entity(1)
{
	if (!mTexture.loadFromFile("Media/Textures/ball.png"))
	{
		std::cout << "Failure: Texture" << std::endl;
	}
	mSprite.setTexture(mTexture);

	this->setVelocity(sf::Vector2f(5, 3));
}


Ball::~Ball()
{
}

sf::FloatRect Ball::getBoundingRect() const
{
	return getWorldTransform().transformRect(mSprite.getGlobalBounds());
}

void Ball::updateCurrent(sf::Time dt, CommandQueue& commands)
{
	Entity::updateCurrent(dt, commands);
}