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
		Ball	= 1 << 4,
		Goal = 1 << 5,
		SoundEffect			= 1 << 6,
		Network				= 1 << 7,

		Projectile = Ball,
	};
}

#endif // BOOK_CATEGORY_HPP
