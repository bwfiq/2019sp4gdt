#pragma once
#include "AnimationBase.h"

class AnimationPanic : public AnimationBase
{
public:
	float currentYTranslation;
	float currentRotationX;
	float currentRotationZ;
	float tempY;
	AnimationPanic();
	virtual ~AnimationPanic();

	Vector3 dir;
	virtual void Update(float dt);
protected:
private:
	//virtual void Render() = 0;
};