#include "AnimationChopping.h"

AnimationChopping::AnimationChopping()
	:AnimationBase()
	,tempY(0)
{
	//Rotate.SetToRotation(90, 1, 0, 0);
	type = A_CHOPPING;
}

AnimationChopping::~AnimationChopping()
{
}

void AnimationChopping::Update(float dt)
{
	Translate.SetToIdentity();
	TranslateOffset.SetToIdentity();
	RevertTranslateOffset.SetToIdentity();
	OffsetRotate.SetToIdentity();
	Rotate.SetToIdentity();
	Scale.SetToIdentity();

	tempY += dt * 10.f;
	//currentRotationX = 20.f * fabsf(sinf(tempY * 0.5f));

	currentRotationZ = 20.f * fabsf(sinf(tempY));

	currentRotationY = -30.f * fabsf(sinf(tempY));
	
	Mtx44 RotateX;
	//RotateX.SetToRotation(currentRotationX, 1, 0, 0);
	Mtx44 RotateZ;
	RotateZ.SetToRotation(currentRotationZ, 0, 0, 1);
	OffsetRotate = OffsetRotate * RotateZ;

	Mtx44 RotateY;
	RotateY.SetToRotation(currentRotationY, 0, 1, 0);
	Rotate = Rotate * RotateY;

	TranslateOffset.SetToTranslation(0, 0.2f, 0);
	RevertTranslateOffset.SetToTranslation(0, -0.2f, 0);

	//Set current transformation
	AnimationBase::Update(dt);
}