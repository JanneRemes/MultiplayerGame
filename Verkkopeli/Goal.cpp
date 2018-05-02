#include <iostream>

#include "Goal.hpp"


Goal::Goal(int id, int hp, int x, int y) : Entity(hp)
{
	playerId = id;
	playerHp = hp;
	posX = x;
	posY = y;
	
	
	if (!textureGoal.loadFromFile("Media/Textures/FinishLine.png"))
	{
		std::cout << "Failure: Texture" << std::endl;
	}
	spriteGoal.setTexture(textureGoal);
	spriteGoal.setTextureRect(sf::IntRect(0, 38, 300, 38));
}