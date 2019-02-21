#include "AnimationTiredWalk.h"

AnimationTiredWalk::AnimationTiredWalk()
	:AnimationBase()
	,vel(0,30,0)
	,tempY(0)
{
	//Rotate.SetToRotation(90, 1, 0, 0);
	currentYTranslation = 0;
	type = A_TIRED;
}

AnimationTiredWalk::~AnimationTiredWalk()
{
}

void AnimationTiredWalk::Update(float dt)
{
	Translate.SetToIdentity();
	TranslateOffset.SetToIdentity();
	RevertTranslateOffset.SetToIdentity();
	OffsetRotate.SetToIdentity();
	Rotate.SetToIdentity();
	Scale.SetToIdentity();

	tempY += dt * 9.5f;
	currentYTranslation = 0.8f * fabs(sinf(tempY));
	currentRotation = 10.f * (cosf(tempY * 0.5f));
	OffsetRotate.SetToRotation(currentRotation, 0, 0, 1);
	
	TranslateOffset.SetToTranslation(0, 1, 0);
	RevertTranslateOffset.SetToTranslation(0, -1, 0);
	Translate.SetToTranslation(0, currentYTranslation, 0);
	//Set current transformation
	AnimationBase::Update(dt);
}