#pragma once
#include "AnimationBase.h"

class AnimationConstructing : public AnimationBase
{
public:
	float currentRotationX;
	float currentRotationY;
	float currentRotationZ;

	float tempY;

	AnimationConstructing();
	virtual ~AnimationConstructing();

	virtual void Update(float dt);
protected:
private:
	//virtual void Render() = 0;
};