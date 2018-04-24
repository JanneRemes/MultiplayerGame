#include <iostream>

#include "Goal.hpp"


Goal::Goal(int id, int hp) : Entity(hp)
{
	playerId = id;
	playerHp = hp;
	
	if (!textureGoal.loadFromFile("Media/Texture/FinishLine.png"))
	{
		std::cout << "Failure: Texture" << std::endl;
	}
	spriteGoal.setTexture(textureGoal);
	spriteGoal.setTextureRect(sf::IntRect(0, 38, 300, 38));
}