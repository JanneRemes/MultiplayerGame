#include <iostream>

#include "Goal.hpp"

#include <Book/Category.hpp>

Goal::Goal(int id, int hp, int x, int y, const FontHolder& fonts) : Entity(hp)
{
	playerId = id;
	playerHp = hp;
	posX = x;
	posY = y;
	
	
	//if (!textureGoal.loadFromFile("Media/Textures/FinishLine.png"))
	//{
	//	std::cout << "Failure: Texture" << std::endl;
	//}
	//spriteGoal.setTexture(textureGoal);
	//spriteGoal.setTextureRect(sf::IntRect(0, 38, 300, 38));

	if (!textureGoal.loadFromFile("Media/Textures/goal.png"))
	{
		std::cout << "Failure: Texture" << std::endl;
	}
	spriteGoal.setTexture(textureGoal);

	std::unique_ptr<TextNode> hpDisplay(new TextNode(fonts, ""));
	mHpDisplay = hpDisplay.get();
	attachChild(std::move(hpDisplay));

	updateTexts();
}

void Goal::updateTexts()
{
	// Display hitpoints
	//if (isDestroyed())
	//	mHealthDisplay->setString("");
	//else
	std::string hpString = std::to_string(playerHp);
	std::cout << "Goal" << playerId << ": " << hpString << "HP" << std::endl;
	mHpDisplay->setString(hpString + " HP");
	mHpDisplay->setPosition(0.f, 50.f);
	mHpDisplay->setRotation(-getRotation());
}

unsigned int Goal::getCategory() const
{
	return Category::Pickup;
}
