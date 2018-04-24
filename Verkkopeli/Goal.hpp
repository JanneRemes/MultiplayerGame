#ifndef GOAL_HPP
#define GOAL_HPP

#include <Book\Entity.hpp>

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>

class Goal : public Entity
{
public:
	Goal(int id, int hp);

	int playerId;
	int playerHp;
	sf::Texture textureGoal;
	sf::Sprite spriteGoal;
};

#endif