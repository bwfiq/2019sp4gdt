#include "AnimationForage.h"

AnimationForage::AnimationForage()
	:AnimationBase()
	,tempY(0)
{
	//Rotate.SetToRotation(90, 1, 0, 0);
	type = A_WALK;
	timer = 0.f;

	turnLeft = true;
	goDown = true;
	turnRight = true;
}

AnimationForage::~AnimationForage()
{
}

void AnimationForage::Update(float dt)
{
	Translate.SetToIdentity();
	TranslateOffset.SetToIdentity();
	RevertTranslateOffset.SetToIdentity();
	OffsetRotate.SetToIdentity();
	Rotate.SetToIdentity();
	Scale.SetToIdentity();

	//currentRotationX = 20.f * fabsf(sinf(tempY * 0.5f));


	static const float Yangle = 30.f;
	static const float Zangle = 20.f;
	if (turnLeft)
	{
		tempY += dt * 7.f;
		currentRotationY = Yangle * (sinf(tempY));
		if (tempY >= Math::HALF_PI)
		{
			turnLeft = false;
			currentRotationY = Yangle;
			tempY = 0.f;
		}
	}
	else if (goDown)
	{
		tempY += dt * 10.f;
		currentRotationZ = -Zangle * (sinf(tempY));
		if (tempY >= Math::HALF_PI)
		{
			currentRotationZ = -Zangle;
			tempY = 0.f;
			goDown = false;
		}
	}
	else if (turnRight)
	{
		tempY += dt * 10.f;
		currentRotationY = Yangle * (cosf(tempY));
		if (tempY >= Math::HALF_PI)
		{
			currentRotationY = 0.f;
			turnRight = false;
			tempY = 0.f;
		}
	}
	else if (goUp)
	{
		tempY += dt * 10.f;
		currentRotationZ = -Zangle * (cosf(tempY));
		if (tempY >= Math::HALF_PI)
		{
			currentRotationZ = 0.f;
			goUp = false;
			tempY = 0.f;
		}
	}
	else //Animation finished, wait abit before start again
	{
		timer += dt;
		if (timer > 0.4f)
		{
			timer = 0.f;
			goDown = true;
			turnLeft = true;
			turnRight = true;
			goUp = true;
			tempY = 0.f;
		}
	}
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