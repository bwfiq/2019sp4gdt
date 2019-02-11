#include "ProjectileManager.h"

ProjectileManager::ProjectileManager()
{
}

ProjectileManager::~ProjectileManager()
{
}

void ProjectileManager::Update(double dt)
{
	for (int i = 0; i < projectileVector.size(); ++i)
	{
		//Do something perhaps
		if (!projectileVector[i]->GetActive())
		{
			Projectiles* temp = projectileVector[i];
			projectileVector.erase(projectileVector.begin() + i);
			delete temp;
			continue; //continue so it doesnt do updating
		}
		//else if its active we update
		projectileVector[i]->Update(dt);
	}
}

void ProjectileManager::AddProjectile(Projectiles::TYPE type, Vector3 position, GameObject * target)
{
	Projectiles * newProjectile = new Projectiles();
	newProjectile->SetMaxForce(2);
	newProjectile->SetMaxSpeed(2);
	newProjectile->SetPosition(position);
	newProjectile->SetActive(true);
	newProjectile->SetTargetPosition(target->pos);
	newProjectile->SetVelocity(0);
	newProjectile->type = type;

	projectileVector.push_back(newProjectile);

}

void ProjectileManager::AddProjectile(Projectiles::TYPE type, Vector3 position, Vector3  target)
{
	std::cout << "Added projectile at position: " << position << std::endl;
	Projectiles * newProjectile = new Projectiles();
	newProjectile->SetMaxForce(0.02);
	newProjectile->SetMaxSpeed(1);
	newProjectile->SetPosition(position);
	newProjectile->SetActive(true);
	newProjectile->SetTargetPosition(target);
	newProjectile->SetVelocity(Vector3());
	newProjectile->type = type;

	projectileVector.push_back(newProjectile);

}

void ProjectileManager::AddProjectile(Projectiles * projectile)
{
	projectileVector.push_back(projectile);
}

void ProjectileManager::AddProjectileHit(Vector3 position)
{
	projectileHits.push_back(position);
}

void ProjectileManager::DeleteAll(void)
{
	//enemyVector.clear();
	while (projectileVector.size() > 0)
	{
		Projectiles *projectiles = projectileVector.back();
		projectiles->SetActive(false);

		delete projectiles;
		projectileVector.pop_back();
	}


}

std::vector<Projectiles*> ProjectileManager::GetProjectileVector(void)
{
	return projectileVector;
}

std::vector<Vector3>& ProjectileManager::GetProjectileHitVector(void)
{
	return projectileHits;
}
