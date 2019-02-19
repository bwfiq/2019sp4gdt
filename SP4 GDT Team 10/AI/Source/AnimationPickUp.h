#pragma once
#include "AnimationBase.h"

class AnimationPickUp : public AnimationBase
{
public:
	float currentRotationY;
	float currentRotationZ;

	float tempY;

	AnimationPickUp();
	virtual ~AnimationPickUp();

	virtual void Update(float dt);
protected:
private:
	//virtual void Render() = 0;
};