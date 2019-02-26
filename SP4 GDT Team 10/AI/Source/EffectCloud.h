#pragma once
#include "EffectMovable.h"
#include <list>
class Mesh;
class EffectTrail;

class EffectCloud : public EffectMovable {
public:
	EffectCloud(Vector3 origPos, float lifeTime = 1, Vector3 startScale = Vector3(1, 1, 1) , Vector3 endScale = Vector3(0.04f, 0.04f, 0.04f));
	virtual ~EffectCloud();

	Vector3 startScale;
	Vector3 endScale;
	float fElapsedTime;
	float fLifetime;
	EffectTrail* trail;

	virtual void Update(float dt);
	virtual void AttachTrail(EffectTrail* trail);
protected:
private:
	//virtual void Render() = 0;
};