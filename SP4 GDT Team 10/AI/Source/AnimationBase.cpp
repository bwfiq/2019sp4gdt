#include "AnimationBase.h"

AnimationBase::AnimationBase()
{
	Translate.SetToIdentity();
	TranslateOffset.SetToIdentity();
	RevertTranslateOffset.SetToIdentity();
	OffsetRotate.SetToIdentity();
	Rotate.SetToIdentity();
	Scale.SetToIdentity();
	CurrentTransformation.SetToIdentity();
	active = true;
	type = A_JUMP;
}

AnimationBase::~AnimationBase()
{
}

Mtx44 AnimationBase::GetCurrentTransformation(void)
{
	return CurrentTransformation;
}

void AnimationBase::Update(float dt)
{
	CurrentTransformation.SetToIdentity();
	CurrentTransformation = CurrentTransformation * Translate  * Rotate * RevertTranslateOffset * OffsetRotate  * TranslateOffset * Scale;
}

void AnimationBase::MultiplyMtx(void)
{
	CurrentTransformation.SetToIdentity();
	CurrentTransformation = CurrentTransformation * Translate  * Rotate * RevertTranslateOffset * OffsetRotate  * TranslateOffset * Scale;
}
