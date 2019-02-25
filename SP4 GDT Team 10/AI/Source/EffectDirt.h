#pragma once
#include "EffectMovable.h"
#include <list>
class Mesh;

class EffectDirt : public EffectMovable {
public:
	EffectDirt(Vector3 origPos, float lifeTime = 1, Vector3 startScale = Vector3(1, 1, 1) , Vector3 endScale = Vector3(0.04f, 0.04f, 0.04f));
	virtual ~EffectDirt();

	Vector3 startScale;
	Vector3 endScale;
	Vector3 torque;
	float fElapsedTime;
	float fLifetime;

	virtual void Update(float dt);
protected:
private:
	//virtual void Render() = 0;
};