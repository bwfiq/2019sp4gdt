#include "AnimationPickUp.h"

AnimationPickUp::AnimationPickUp()
	:AnimationBase()
	,tempY(0)
{
	//Rotate.SetToRotation(90, 1, 0, 0);
	type = A_WALK;
}

AnimationPickUp::~AnimationPickUp()
{
}

void AnimationPickUp::Update(float dt)
{
	Translate.SetToIdentity();
	TranslateOffset.SetToIdentity();
	RevertTranslateOffset.SetToIdentity();
	OffsetRotate.SetToIdentity();
	Rotate.SetToIdentity();
	Scale.SetToIdentity();

	tempY += dt * 15.5f;
	currentRotationY = 10.f * (cosf(tempY));

	currentRotationZ = 10.f * (sinf(tempY * 0.5f));
	
	Mtx44 RotateY;
	RotateY.SetToRotation(currentRotationY, 0, 1, 0);
	Mtx44 RotateZ;
	RotateZ.SetToRotation(45 + currentRotationZ, 0, 0, 1);
	Rotate = Rotate * RotateZ * RotateY;

	//Set current transformation
	AnimationBase::Update(dt);
}