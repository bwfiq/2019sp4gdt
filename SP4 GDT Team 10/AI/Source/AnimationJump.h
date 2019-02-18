#pragma once
#include "AnimationBase.h"

class AnimationJump : public AnimationBase
{
public:
	float currentYTranslation;
	float tempY;
	AnimationJump();
	virtual ~AnimationJump();

	Vector3 vel;
	virtual void Update(float dt);
protected:
private:
	//virtual void Render() = 0;
};