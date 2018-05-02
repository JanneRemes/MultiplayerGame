#ifndef BOOK_CATEGORY_HPP
#define BOOK_CATEGORY_HPP


// Entity/scene node category, used to dispatch commands
namespace Category
{
	enum Type
	{
		None				= 0,
		SceneAirLayer		= 1 << 0,
		PlayerBat		= 1 << 1,
		EnemyBat		= 1 << 2,
		Pickup				= 1 << 3,
		AlliedProjectile	= 1 << 4,
		EnemyProjectile		= 1 << 5,
		ParticleSystem		= 1 << 6,
		SoundEffect			= 1 << 7,
		Network				= 1 << 8,

		Projectile = AlliedProjectile | EnemyProjectile,
	};
}

#endif // BOOK_CATEGORY_HPP
