#include "EffectDirt.h"
#include "SceneData.h"
#include "EasingStyle.h"

EffectDirt::EffectDirt(Vector3 origPos, float lifeTime, Vector3 startScale, Vector3 endScale) :
	fElapsedTime(0)
	, fLifetime(lifeTime)
{
	mesh = SceneData::GetInstance()->GetMesh("effect_dirt");
	pos = origPos;
	bLightEnabled = true;
	this->startScale = scale = startScale;
	this->endScale = endScale;
	Vector3 randVel = Vector3(Math::RandFloatMinMax(-1, 1), Math::RandFloatMinMax(1, 2), Math::RandFloatMinMax(-1, 1)).Normalized();
	float randVelMagnitude = Math::RandFloatMinMax(1, 4) / lifeTime;
	vel = randVel * randVelMagnitude;
	acc = Vector3(0, -randVel.y, 0) *randVelMagnitude * 2;
	rotation.Set(Math::RandInt(), Math::RandInt(), Math::RandInt());
	torque = rotation.Normalized() * 500 * randVelMagnitude;
}

EffectDirt::~EffectDirt()
{
}

void EffectDirt::Update(float dt)
{
	EffectMovable::Update(dt);
	fElapsedTime += dt;
	rotation += torque * dt;
	float alpha = Math::Min(1.f, fElapsedTime / fLifetime);
	scale = startScale.lerped(endScale, alpha);
	if (fElapsedTime > fLifetime)
		this->bIsDone = true;
}
