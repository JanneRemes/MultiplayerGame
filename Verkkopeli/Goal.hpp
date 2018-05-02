#ifndef GOAL_HPP
#define GOAL_HPP

#include <Book\Entity.hpp>

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>

class Goal : public Entity
{
public:
	Goal(int id, int hp, int x, int y);

	int playerId = 0;
	int playerHp = 0;
	int posX = 0;
	int posY = 0;
	sf::Texture textureGoal;
	sf::Sprite spriteGoal;
	
};

#endif