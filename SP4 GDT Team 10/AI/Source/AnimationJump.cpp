#include "AnimationJump.h"

AnimationJump::AnimationJump()
	:AnimationBase()
	,vel(0,30,0)
	,tempY(0)
{
	//Rotate.SetToRotation(90, 1, 0, 0);
	currentYTranslation = 0;
	type = A_JUMP;
}

AnimationJump::~AnimationJump()
{
}

void AnimationJump::Update(float dt)
{
	Translate.SetToIdentity();
	TranslateOffset.SetToIdentity();
	RevertTranslateOffset.SetToIdentity();
	OffsetRotate.SetToIdentity();
	Rotate.SetToIdentity();
	Scale.SetToIdentity();

	tempY += dt * 10;
	currentYTranslation = 1.f * fabs(sinf(tempY));
	//Rotate.SetToRotation(90, 1, 0, 0);
	//TranslateOffset.SetToTranslation(0, 1, 0);
	//RevertTranslateOffset.SetToTranslation(0, -1, 0);
	Translate.SetToTranslation(0, currentYTranslation, 0);
	if (tempY > Math::PI)
		active = false;
	//Set current transformation
	AnimationBase::Update(dt);
}