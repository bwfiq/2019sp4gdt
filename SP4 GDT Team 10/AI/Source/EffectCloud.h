#pragma once
#include "EffectMovable.h"
#include <list>
class Mesh;

class EffectCloud : public EffectMovable {
public:
	EffectCloud(Vector3 origPos, float lifeTime = 1, Vector3 startScale = Vector3(1, 1, 1) , Vector3 endScale = Vector3(0.03f, 0.03f, 0.03f));
	virtual ~EffectCloud();

	Vector3 startScale;
	Vector3 endScale;
	float fElapsedTime;
	float fLifetime;

	virtual void Update(float dt);
protected:
private:
	//virtual void Render() = 0;
};