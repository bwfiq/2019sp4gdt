#include "AnimationBase.h"

AnimationBase::AnimationBase()
{
	Translate.SetToIdentity();
	TranslateOffset.SetToIdentity();
	RevertTranslateOffset.SetToIdentity();
	Rotate.SetToIdentity();
	Scale.SetToIdentity();
	CurrentTransformation.SetToIdentity();
	active = true;
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
	CurrentTransformation = CurrentTransformation * Translate * TranslateOffset * RevertTranslateOffset * Rotate  * TranslateOffset * Scale;
}
