#ifndef PROJECTILES_H
#define PROJECTILES_H

#include "Vector3.h"
#include "GameObject.h"
#include "Mesh.h"

class ProjectileManager;

class Projectiles
{
public :
	Projectiles();
	~Projectiles();

	enum TYPE
	{
		DEFAULT,
		NUM_TYPE,
	};
	TYPE type;

	//Get and Set positions
	void SetPosition(Vector3 position);
	Vector3 GetPosition(void);

	//Get and Set targetPosition
	void SetTargetPosition(Vector3 targetPosition);
	Vector3 GetTargetPosition(void);

	//Get and Set acceleration
	void SetAcceleration(Vector3 acceleration);
	Vector3 GetAcceleration(void);

	//Get and Set maxSpeed
	void SetMaxSpeed(float maxSpeed);
	float GetMaxSpeed(void);

	//Get and Set maxForce
	void SetMaxForce(float maxForce);
	float GetMaxForce(void);

	//Get and Set velocity
	void SetVelocity(Vector3 velocity);
	Vector3 GetVelocity(void);

	//Get and set Active
	void SetActive(const bool active);
	bool GetActive(void);

	//Get and set target
	void SetTarget(GameObject*go);
	GameObject * GetTarget(void);

	void SetCollisionThreshhold(float value);
	float GetCollisionThreshhold();

	//Get and set damage
	void SetDamage(int damage);
	int GetDamage(void);

	//Homing functions
	void CheckForCollision(void);
	void Update(double dt);
	void ApplyForce(Vector3 force);

	void Seek(Vector3 target);

private:

	//The position of the projectile
	Vector3 position;
	//The target of the projectile
	Vector3 targetPosition;
	//The acceleration of the projectile
	Vector3 acceleration;
	//The maxSpeed of the projectile
	float maxSpeed;
	//The maxTurning force of the projectile
	float maxForce;
	//The velocity of the projectile
	Vector3 velocity;

	//The projectile's mesh
	Mesh* projectileMesh;

	//The area of effect of this particle
	float collisionThreshold;

	//Whether this particle is active
	bool active;

	GameObject * target;
	Vector3 actualTarget;

	int damage;
	


};


#endif