#pragma once
#include "EffectBase.h"
#include <list>
class Camera;
class Mesh;

class EffectTrail : public EffectBase {
public:
	EffectTrail(Camera* cam);
	virtual ~EffectTrail();

	std::list<Vector3> trailPoints;
	float fLifetime;
	float fLineThickness;
	int iTotalPoints;
	Camera* cameraObj;

	virtual void Update(float dt);
protected:
private:
	//virtual void Render() = 0;
};