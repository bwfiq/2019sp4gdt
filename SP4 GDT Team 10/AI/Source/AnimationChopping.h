#pragma once
#include "AnimationBase.h"

class AnimationChopping : public AnimationBase
{
public:
	float currentRotationX;
	float currentRotationY;
	float currentRotationZ;

	float tempY;

	AnimationChopping();
	virtual ~AnimationChopping();

	virtual void Update(float dt);
protected:
private:
	//virtual void Render() = 0;
};