#include "AnimationWalk.h"

AnimationWalk::AnimationWalk()
	:AnimationBase()
	,vel(0,30,0)
	,tempY(0)
{
	//Rotate.SetToRotation(90, 1, 0, 0);
	currentYTranslation = 0;
	type = A_WALK;
}

AnimationWalk::~AnimationWalk()
{
}

void AnimationWalk::Update(float dt)
{
	////Gravity
	//vel.y -= 9.8f * dt;
	tempY += dt * 9.5f;
	currentYTranslation = 0.8f * fabs(sinf(tempY));
	currentRotation = 15.f * (cosf(tempY));
	OffsetRotate.SetToRotation(currentRotation, 1, 0, 0);
	
	TranslateOffset.SetToTranslation(0, 1, 0);
	RevertTranslateOffset.SetToTranslation(0, -1, 0);
	Translate.SetToTranslation(0, currentYTranslation, 0);
	//Set current transformation
	AnimationBase::Update(dt);
}