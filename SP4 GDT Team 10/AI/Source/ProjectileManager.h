#ifndef PROJECTILE_MANAGER_H
#define PROJECTILE_MANAGER_H

#include "Projectiles.h"
#include "SingletonTemplate.h"
#include "GameObject.h"

class ProjectileManager : public Singleton<ProjectileManager>
{
	//friend Singleton<ProjectileManager>;

public:
	ProjectileManager();
	~ProjectileManager();

	//Update function for the enemies in the vector so i can update all the projectiles in the game
	void Update(double dt);

	//Add projectile

	void AddProjectile(Projectiles::TYPE type, Vector3 position, GameObject* target);
	void AddProjectile(Projectiles::TYPE type, Vector3 position, Vector3 target);
	void AddProjectile(Projectiles* projectile);

	//Vector3
	void AddProjectileHit(Vector3 position);

	void DeleteAll(void);

	std::vector<Projectiles*> GetProjectileVector(void);

	std::vector<Vector3>& GetProjectileHitVector(void);
private:
	std::vector<Projectiles*>projectileVector;
	std::vector<Vector3> projectileHits;
};



#endif