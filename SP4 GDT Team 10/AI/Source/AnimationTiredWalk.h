#pragma once
#include "AnimationBase.h"

class AnimationTiredWalk : public AnimationBase
{
public:
	float currentYTranslation;
	float currentRotation;
	float tempY;
	AnimationTiredWalk();
	virtual ~AnimationTiredWalk();

	Vector3 vel;
	virtual void Update(float dt);
protected:
private:
	//virtual void Render() = 0;
};