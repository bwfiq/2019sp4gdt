#include "Projectiles.h"
#include "ProjectileManager.h"

void Projectiles::SetVelocity(Vector3 velocity)
{
	this->velocity = velocity;
}

Vector3 Projectiles::GetVelocity(void)
{
	return velocity;
}

void Projectiles::SetActive(const bool active)
{
	this->active = active;
}

bool Projectiles::GetActive(void)
{
	return active;
}

void Projectiles::SetTarget(GameObject * go)
{
	this->target = go;
}

GameObject * Projectiles::GetTarget(void)
{
	return target;
}

void Projectiles::SetCollisionThreshhold(float value)
{
	collisionThreshold = value;
}

float Projectiles::GetCollisionThreshhold()
{
	return collisionThreshold;
}

void Projectiles::SetDamage(int damage)
{
	this->damage = damage;
}

int Projectiles::GetDamage(void)
{
	return damage;
}

void Projectiles::CheckForCollision(void)
{
	if ((targetPosition - position).LengthSquared() < collisionThreshold *collisionThreshold)
	{
		//Enemy * thatEnemy = dynamic_cast<Enemy*>(target);
		//if (thatEnemy)
		//{
		//	if (thatEnemy->IsSkeleton())
		//	{
		//		thatEnemy->IsSkeleton()->SetHP(thatEnemy->IsSkeleton()->GetHP() - damage);
		//	}
		//	else	if (thatEnemy->IsSkeletonArcher())
		//	{
		//		thatEnemy->IsSkeletonArcher()->SetHP(thatEnemy->IsSkeletonArcher()->GetHP() - damage);
		//	}
		//	else if (thatEnemy->IsSkeletonChief())
		//	{
		//		thatEnemy->IsSkeletonChief()->SetHP(thatEnemy->IsSkeletonChief()->GetHP() - damage);
		//	}
		//}
		//ProjectileManager::GetInstance()->AddProjectileHit(thatEnemy->pos);

	//	if(target != NULL)
	//		target->health = Math::Max(0, target->health - damage);
		active = false;
	}
}

void Projectiles::Update(double dt)
{
	CheckForCollision();

	if (target != NULL)
	{
		targetPosition = target->pos;
		if (!target->active)
			target = NULL;
	}
	Seek(targetPosition);
	//Update velocity
	velocity += acceleration;

	if (velocity.Length() > maxSpeed)
	{
		velocity.Normalize();
		velocity *= maxSpeed;
	}

	position += velocity;

	acceleration.SetZero();
}

void Projectiles::ApplyForce(Vector3 force)
{
	acceleration += force;
}

void Projectiles::Seek(Vector3 target)
{
	Vector3 desired = target - position;

	try {
		desired = desired.Normalize() * maxSpeed;
	}
	catch(DivideByZero exception)
	{
		std::cout << "Projectile is at or close to target position" << std::endl;
	}

	Vector3 steer = desired - velocity;
	
	if (steer.Length() > maxForce)
	{
		steer.Normalize();
		steer *= maxForce;
	}

	ApplyForce(steer);

}

Projectiles::Projectiles()
{
	acceleration.SetZero();
	position.SetZero();
	velocity.SetZero();
	target = NULL;
	maxSpeed = 0;
	maxForce = 0;
	targetPosition.SetZero();
	collisionThreshold = 2;
}

Projectiles::~Projectiles()
{
}

void Projectiles::SetPosition(Vector3 position)
{
	this->position = position;
}

Vector3 Projectiles::GetPosition(void)
{
	return position;
}

void Projectiles::SetTargetPosition(Vector3 targetPosition)
{
	this->targetPosition = targetPosition;
}

Vector3 Projectiles::GetTargetPosition(void)
{
	return targetPosition;
}

void Projectiles::SetAcceleration(Vector3 acceleration)
{
	this->acceleration = acceleration;

}

Vector3 Projectiles::GetAcceleration(void)
{
	return acceleration;
}

void Projectiles::SetMaxSpeed(float maxSpeed)
{
	this->maxSpeed = maxSpeed;
}

float Projectiles::GetMaxSpeed(void)
{
	return maxSpeed;
}

void Projectiles::SetMaxForce(float maxForce)
{
	this->maxForce = maxForce;
}

float Projectiles::GetMaxForce(void)
{
	return maxForce;
}
