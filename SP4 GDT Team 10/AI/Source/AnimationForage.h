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

	float timer;
protected:
	bool turnLeft;
	bool goDown;
	bool turnRight;
	bool goUp;
private:
	//virtual void Render() = 0;
};