#ifndef GAME_CATEGORY_HPP
#define GAME_CATEGORY_HPP


// Entity/scene node category, used to dispatch commands
namespace Category
{
	enum Type
	{
		None				= 0,
		Entity		        = 1 << 0,

		//Creature = PlayerCreature | AlliedCreature | EnemyCreature,
		//Projectile = AlliedProjectile | EnemyProjectile,
	};
}

#endif // GAME_CATEGORY_HPP
