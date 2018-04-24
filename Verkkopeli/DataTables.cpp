#include <Book/DataTables.hpp>
#include <Book/PlayerBat.hpp>
#include <Book/Projectile.hpp>
#include <Book/Pickup.hpp>


// For std::bind() placeholders _1, _2, ...
using namespace std::placeholders;

std::vector<PlayerBatData> initializePlayerBatData()
{
	std::vector<PlayerBatData> data(PlayerBat::TypeCount);

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

	data[Projectile::EnemyBullet].damage = 10;
	data[Projectile::EnemyBullet].speed = 300.f;
	data[Projectile::EnemyBullet].texture = Textures::Entities;
	data[Projectile::EnemyBullet].textureRect = sf::IntRect(178, 64, 3, 14);

	data[Projectile::Missile].damage = 200;
	data[Projectile::Missile].speed = 150.f;
	data[Projectile::Missile].texture = Textures::Entities;
	data[Projectile::Missile].textureRect = sf::IntRect(160, 64, 15, 32);

	return data;
}

std::vector<PickupData> initializePickupData()
{
	std::vector<PickupData> data(Pickup::TypeCount);
	
	data[Pickup::HealthRefill].texture = Textures::Entities;
	data[Pickup::HealthRefill].textureRect = sf::IntRect(0, 64, 40, 40);
	data[Pickup::HealthRefill].action = [] (PlayerBat& a) { a.repair(25); };
	
	data[Pickup::MissileRefill].texture = Textures::Entities;
	data[Pickup::MissileRefill].textureRect = sf::IntRect(40, 64, 40, 40);
	data[Pickup::MissileRefill].action = std::bind(&PlayerBat::collectMissiles, _1, 3);
	
	data[Pickup::FireSpread].texture = Textures::Entities;
	data[Pickup::FireSpread].textureRect = sf::IntRect(80, 64, 40, 40);
	data[Pickup::FireSpread].action = std::bind(&PlayerBat::increaseSpread, _1);
	
	data[Pickup::FireRate].texture = Textures::Entities;
	data[Pickup::FireRate].textureRect = sf::IntRect(120, 64, 40, 40);
	data[Pickup::FireRate].action = std::bind(&PlayerBat::increaseFireRate, _1);

	return data;
}