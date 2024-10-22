#include "AnimationWalk.h"

AnimationWalk::AnimationWalk()
	:AnimationBase()
	,vel(0,30,0)
	,tempY(0)
{
	//Rotate.SetToRotation(90, 1, 0, 0);
	currentYTranslation = 0;
	type = A_WALK;
	rotationMagnitude = 10.f;
}

AnimationWalk::AnimationWalk(float rotationMag)
	:AnimationBase()
	, vel(0, 30, 0)
	, tempY(0)
{
	//Rotate.SetToRotation(90, 1, 0, 0);
	currentYTranslation = 0;
	type = A_WALK;
	rotationMagnitude = rotationMag;
}

AnimationWalk::~AnimationWalk()
{
}

void AnimationWalk::Update(float dt)
{
	Translate.SetToIdentity();
	TranslateOffset.SetToIdentity();
	RevertTranslateOffset.SetToIdentity();
	OffsetRotate.SetToIdentity();
	Rotate.SetToIdentity();
	Scale.SetToIdentity();

	tempY += dt * 9.5f;
	currentYTranslation = 0.8f * fabs(sinf(tempY));
	currentRotation = rotationMagnitude * (cosf(tempY));
	OffsetRotate.SetToRotation(currentRotation, 1, 0, 0);
	
	TranslateOffset.SetToTranslation(0, 1, 0);
	RevertTranslateOffset.SetToTranslation(0, -1, 0);
	Translate.SetToTranslation(0, currentYTranslation, 0);
	//CSoundEngine::GetInstance()->PlayASound("step",true);
	//Set current transformation
	AnimationBase::Update(dt);
}