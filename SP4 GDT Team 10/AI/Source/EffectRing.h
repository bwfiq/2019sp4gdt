#pragma once
#include "EffectMovable.h"

class EffectRing : public EffectBase {
public:
	EffectRing(Vector3 origPos, float lifeTime = 1, Vector3 startScale = Vector3(0.04f, 0.04f, 0.04f) , Vector3 endScale = Vector3(2, 2, 2), EffectBase* lockedTo = NULL);
	virtual ~EffectRing();

	Vector3 startScale;
	Vector3 endScale;
	float fElapsedTime;
	float fLifetime;

	EffectBase* lockedTo;

	virtual void Update(float dt);
protected:
private:
	//virtual void Render() = 0;
};