#pragma once
#include "AnimationBase.h"

class AnimationForage : public AnimationBase
{
public:
	float currentRotationX;
	float currentRotationY;
	float currentRotationZ;

	float tempY;

	AnimationForage();
	virtual ~AnimationForage();

	virtual void Update(float dt);
protected:
private:
	//virtual void Render() = 0;
};