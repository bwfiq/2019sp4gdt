#pragma once
#include "EffectBase.h"
#include <list>
class Mesh;

class EffectTrail : public EffectBase {
public:
	EffectTrail(EffectBase* toTrace);
	virtual ~EffectTrail();

	std::list<Vector3> trailPoints;
	float fLifetime;
	float fLineThickness;
	int iTotalPoints;
	EffectBase* toTrace;

	virtual void Update(float dt);
	virtual void SetTrailLifetime(float lifeTime);
protected:
private:
	//virtual void Render() = 0;
};