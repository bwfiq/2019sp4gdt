#include "EffectCloud.h"
#include "SceneData.h"
#include "EasingStyle.h"

EffectCloud::EffectCloud(Vector3 origPos, float lifeTime, Vector3 startScale, Vector3 endScale) :
	fElapsedTime(0)
	, fLifetime(lifeTime)
{
	mesh = SceneData::GetInstance()->GetMesh("effect_cloud");
	pos = origPos;
	bLightEnabled = true;
	this->startScale = scale = startScale;
	this->endScale = endScale;
	Vector3 randVel = Vector3(Math::RandFloatMinMax(-1, 1), Math::RandFloatMinMax(0, 1), Math::RandFloatMinMax(-1, 1)).Normalized();
	float randVelMagnitude = Math::RandFloatMinMax(1, 4) / lifeTime;
	vel = randVel * randVelMagnitude;
	acc = -randVel * randVelMagnitude * 2;
	rotation.Set(Math::RandInt() % 361, Math::RandInt() % 361, Math::RandInt() % 361);
}

EffectCloud::~EffectCloud()
{
}

void EffectCloud::Update(float dt)
{
	EffectMovable::Update(dt);
	fElapsedTime += dt;
	float alpha = Math::Min(1.f, fElapsedTime / fLifetime);
	scale = startScale.lerped(endScale, EasingStyle::easeOutSine(alpha, 0, 1, 1));
	if (alpha > 0.5f)
	{
		acc.Set(0, 1, 0);
	}
	if (fElapsedTime > fLifetime)
		this->bIsDone = true;
}
