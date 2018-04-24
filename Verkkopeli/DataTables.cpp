#include <Book/DataTables.hpp>
#include <Book/PlayerBat.hpp>
#include <Book/Projectile.hpp>
#include <Book/Pickup.hpp>


// For std::bind() placeholders _1, _2, ...
using namespace std::placeholders;

std::vector<PlayerBatData> initializePlayerBatData()
{
	std::vector<PlayerBatData> data(PlayerBat::TypeCount);

	data[PlayerBat::Player1].hitpoints = 100;
	data[PlayerBat::Player1].speed = 200.f;
	data[PlayerBat::Player1].fireInterval = sf::seconds(1);
	data[PlayerBat::Player1].texture = Textures::Player1;
	data[PlayerBat::Player1].textureRect = sf::IntRect(0, 0, 48, 64); // TODO fix shape
	data[PlayerBat::Player1].hasRollAnimation = true;

	data[PlayerBat::Player2].hitpoints = 100;
	data[PlayerBat::Player2].speed = 200.f;
	data[PlayerBat::Player2].fireInterval = sf::seconds(1);
	data[PlayerBat::Player2].texture = Textures::Player2;
	data[PlayerBat::Player2].textureRect = sf::IntRect(0, 0, 48, 64);
	data[PlayerBat::Player2].hasRollAnimation = true;

	data[PlayerBat::Eagle].hitpoints = 100;
	data[PlayerBat::Eagle].speed = 200.f;
	data[PlayerBat::Eagle].fireInterval = sf::seconds(1);
	data[PlayerBat::Eagle].texture = Textures::Entities;
	data[PlayerBat::Eagle].textureRect = sf::IntRect(0, 0, 48, 64);
	data[PlayerBat::Eagle].hasRollAnimation = true;

	data[PlayerBat::Raptor].hitpoints = 20;
	data[PlayerBat::Raptor].speed = 80.f;
	data[PlayerBat::Raptor].texture = Textures::Entities;
	data[PlayerBat::Raptor].textureRect = sf::IntRect(144, 0, 84, 64);
	data[PlayerBat::Raptor].directions.push_back(Direction(+45.f, 80.f));
	data[PlayerBat::Raptor].directions.push_back(Direction(-45.f, 160.f));
	data[PlayerBat::Raptor].directions.push_back(Direction(+45.f, 80.f));
	data[PlayerBat::Raptor].fireInterval = sf::Time::Zero;
	data[PlayerBat::Raptor].hasRollAnimation = false;

	data[PlayerBat::Avenger].hitpoints = 40;
	data[PlayerBat::Avenger].speed = 50.f;
	data[PlayerBat::Avenger].texture = Textures::Entities;
	data[PlayerBat::Avenger].textureRect = sf::IntRect(228, 0, 60, 59);
	data[PlayerBat::Avenger].directions.push_back(Direction(+45.f,  50.f));
	data[PlayerBat::Avenger].directions.push_back(Direction(  0.f,  50.f));
	data[PlayerBat::Avenger].directions.push_back(Direction(-45.f, 100.f));
	data[PlayerBat::Avenger].directions.push_back(Direction(  0.f,  50.f));
	data[PlayerBat::Avenger].directions.push_back(Direction(+45.f,  50.f));
	data[PlayerBat::Avenger].fireInterval = sf::seconds(2);
	data[PlayerBat::Avenger].hasRollAnimation = false;

	return data;
}

std::vector<ProjectileData> initializeProjectileData()
{
	std::vector<ProjectileData> data(Projectile::TypeCount);

	data[Projectile::AlliedBullet].damage = 10;
	data[Projectile::AlliedBullet].speed = 300.f;
	data[Projectile::AlliedBullet].texture = Textures::Entities;
	data[Projectile::AlliedBullet].textureRect = sf::IntRect(175, 64, 3, 14);

	return data;
}

std::vector<PickupData> initializePickupData()
{
	std::vector<PickupData> data(Pickup::TypeCount);
	
	data[Pickup::HealthRefill].texture = Textures::Entities;
	data[Pickup::HealthRefill].textureRect = sf::IntRect(0, 64, 40, 40);
	data[Pickup::HealthRefill].action = [] (PlayerBat& a) { a.repair(25); };

	return data;
}