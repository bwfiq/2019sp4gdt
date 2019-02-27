#include "AnimationPanic.h"
#include "EasingStyle.h"

AnimationPanic::AnimationPanic()
	:AnimationBase()
	,tempY(0)
{
	//Rotate.SetToRotation(90, 1, 0, 0);
	currentYTranslation = 0;
	type = A_PANIC;
}

AnimationPanic::~AnimationPanic()
{
}

void AnimationPanic::Update(float dt)
{
	Translate.SetToIdentity();
	TranslateOffset.SetToIdentity();
	RevertTranslateOffset.SetToIdentity();
	OffsetRotate.SetToIdentity();
	Rotate.SetToIdentity();
	Scale.SetToIdentity();

	tempY += dt * 9.5f;
	currentYTranslation = 0.8f * fabs(sinf(tempY));
	currentRotationX = 15.f * (cosf(tempY));

	currentRotationZ = 20.f * (cosf(tempY * 1.1f)) * fabsf(sinf(tempY));

	dir.x = cosf(tempY);
	dir.z = -sinf(tempY);
	dir = dir.Normalized() * 0.5f;

	Mtx44 OffsetRotateX;
	OffsetRotateX.SetToRotation(currentRotationX, 1, 0, 0);
	Mtx44 OffsetRotateZ;
	OffsetRotateZ.SetToRotation(currentRotationZ, 0, 0, 1);
	OffsetRotate = OffsetRotate * OffsetRotateX * OffsetRotateZ;
	
	TranslateOffset.SetToTranslation(0, 1, 0);
	RevertTranslateOffset.SetToTranslation(0, -1, 0);
	Translate.SetToTranslation(dir.x, currentYTranslation, dir.z);
	//Set current transformation
	AnimationBase::Update(dt);

	
}