#include <Book/DataTables.hpp>
#include <Book/PlayerBat.hpp>
#include <Book/PlayerGoal.hpp>
#include <Book/Projectile.hpp>
#include <Book/Pickup.hpp>


// For std::bind() placeholders _1, _2, ...
using namespace std::placeholders;

std::vector<PlayerBatData> initializePlayerBatData()
{
	std::vector<PlayerBatData> data(PlayerBat::TypeCount);

	data[PlayerBat::Player1].hitpoints = 100;
	data[PlayerBat::Player1].speed = 150.f;
	data[PlayerBat::Player1].fireInterval = sf::seconds(1);
	data[PlayerBat::Player1].texture = Textures::Player1;
	data[PlayerBat::Player1].textureRect = sf::IntRect(0, 0, 150, 24);
	data[PlayerBat::Player1].hasRollAnimation = true;

	data[PlayerBat::Player2].hitpoints = 100;
	data[PlayerBat::Player2].speed = 150.f;
	data[PlayerBat::Player2].fireInterval = sf::seconds(1);
	data[PlayerBat::Player2].texture = Textures::Player2;
	data[PlayerBat::Player2].textureRect = sf::IntRect(0, 0, 150, 24);
	data[PlayerBat::Player2].hasRollAnimation = true;

	return data;
}

std::vector<PlayerGoalData> initializePlayerGoalData()
{
	std::vector<PlayerGoalData> data(PlayerGoal::TypeCount);

	data[PlayerGoal::Goal1].hitpoints = 5;
	data[PlayerGoal::Goal1].speed = 0.f;
	data[PlayerGoal::Goal1].fireInterval = sf::seconds(1);
	data[PlayerGoal::Goal1].texture = Textures::Goal;
	data[PlayerGoal::Goal1].textureRect = sf::IntRect(0, 0, 164, 38);
	data[PlayerGoal::Goal1].hasRollAnimation = true;

	data[PlayerGoal::Goal2].hitpoints = 5;
	data[PlayerGoal::Goal2].speed = 0.f;
	data[PlayerGoal::Goal2].fireInterval = sf::seconds(1);
	data[PlayerGoal::Goal2].texture = Textures::Goal;
	data[PlayerGoal::Goal2].textureRect = sf::IntRect(0, 0, 164, 38);
	data[PlayerGoal::Goal2].hasRollAnimation = true;

	return data;
}

std::vector<ProjectileData> initializeProjectileData()
{
	std::vector<ProjectileData> data(Projectile::TypeCount);

	data[0].damage = 10;
	data[0].speed = 300.f;
	data[0].texture = Textures::Ball;
	data[0].textureRect = sf::IntRect(0, 0, 24, 24);

	return data;
}

std::vector<PickupData> initializePickupData()
{
	std::vector<PickupData> data(Pickup::TypeCount);

	data[Pickup::Ball].speed = 50.f;
	data[Pickup::Ball].texture = Textures::Ball;
	data[Pickup::Ball].textureRect = sf::IntRect(0, 24, 24, 24);

	return data;
}