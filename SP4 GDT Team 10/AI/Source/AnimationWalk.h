#pragma once
#include "AnimationBase.h"

class AnimationWalk : public AnimationBase
{
public:
	float currentYTranslation;
	float currentRotation;
	float rotationMagnitude;
	float tempY;
	AnimationWalk();
	AnimationWalk(float rotationMag);
	virtual ~AnimationWalk();

	Vector3 vel;
	virtual void Update(float dt);
protected:
private:
	//virtual void Render() = 0;
};