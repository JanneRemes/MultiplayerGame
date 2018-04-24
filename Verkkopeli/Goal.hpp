#ifndef GOAL_HPP
#define GOAL_HPP

#include <Book\Entity.hpp>

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>

class Goal : public Entity
{
public:
	Goal(int id, int hp, int x, int y);

	int playerId;
	int playerHp;
	int posX;
	int posY;
	sf::Texture textureGoal;
	sf::Sprite spriteGoal;
	
};

#endif