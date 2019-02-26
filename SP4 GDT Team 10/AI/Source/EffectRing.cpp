#include "EffectRing.h"
#include "SceneData.h"
#include "EasingStyle.h"

EffectRing::EffectRing(Vector3 origPos, float lifeTime, Vector3 startScale, Vector3 endScale, EffectBase* lockedTo) :
	fElapsedTime(0)
	, fLifetime(lifeTime)
	, lockedTo(lockedTo)
{
	mesh = SceneData::GetInstance()->GetMesh("effect_ring");
	if (lockedTo)
	{
		pos = lockedTo->pos;
	}
	else
	{
		pos = origPos;
	}
	bLightEnabled = true;
	this->startScale = scale = startScale;
	this->endScale = endScale;
	this->fAlpha = 1;
}

EffectRing::~EffectRing()
{
}

void EffectRing::Update(float dt)
{
	SceneData* SD = SceneData::GetInstance();
	fElapsedTime += dt;
	float alpha = Math::Min(1.f, fElapsedTime / fLifetime);
	float easedAlpha = EasingStyle::easeOutCubic(alpha, 0, 1, 1);
	scale = startScale.lerped(endScale, easedAlpha);
	this->fAlpha = Math::lerp(1.f, 0.f, easedAlpha);
	if (lockedTo)
	{
		pos = lockedTo->pos;
	}
	if (fElapsedTime > fLifetime)
	{
		this->bIsDone = true;
	}
	else
	{
		Vector3 camPos = SD->GetCamPosition();
		Vector3 effToCamDir = (camPos - pos).Normalized();
		float rot_y = atan2f(effToCamDir.x, effToCamDir.z);
	}
}
