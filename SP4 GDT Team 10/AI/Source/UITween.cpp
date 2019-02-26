#include "UITween.h"
#include "UIBase.h"
#include "EasingStyle.h"

static const Vector3 DISABLE_TWEEN(-1, -1, -1); // if left alone, will not tween; default value for goal

UITween::UITween(UIBase* ui, float duration, UI_EASINGSTYLE easingStyle, UI_EASINGDIRECTION easingDirection, int repeatCount, /*bool reverses,*/ float delayTime) :
	fDuration(duration)
	, easingStyle(easingStyle)
	, easingDirection(easingDirection)
	, iRepeatCount(0)
	, iRepeatRequired(repeatCount)
	//, bReverses(reverses)
	, bIsDone(false)
	, ui(ui)
{
	fElapsedTime -= delayTime; // adds delay equal to delayTime before animation
	properties_start["pos"] = ui->pos;
	properties_start["scale"] = ui->scale;
	properties_goal["pos"] = DISABLE_TWEEN;
	properties_goal["scale"] = DISABLE_TWEEN;
	switch (easingStyle)
	{
	case ES_BACK:
		easingFunction = EasingStyle::easeOutBack;
	}
}

UITween::~UITween()
{
}

void UITween::SetElapsedTime(float time)
{
	this->fElapsedTime = time;
}

bool UITween::IsDone()
{
	return this->bIsDone;
}

void UITween::SetIsDone(bool done)
{
	this->bIsDone = done;
}

void UITween::Update(float dt)
{
	if (bIsDone)
	{
		ui->pos = properties_goal["pos"];
		ui->scale = properties_goal["scale"];
		return;
	}
	fElapsedTime += dt;
	if (fElapsedTime > 0)
	{
		float alpha_linear = Math::Min(fElapsedTime / fDuration, 1.f);
		float alpha_eased = alpha_linear;
		if (this->easingStyle != ES_LINEAR)
		{
			alpha_eased = this->easingFunction(alpha_linear, 0, 1, 1);
		}
		if (properties_goal["pos"] != DISABLE_TWEEN)//probably could be optimised
		{
			ui->pos = properties_start["pos"].lerped(properties_goal["pos"], alpha_eased);
		}
		if (properties_goal["scale"] != DISABLE_TWEEN)//probably could be optimised
		{
			ui->scale = properties_start["scale"].lerped(properties_goal["scale"], alpha_eased);
		}
	}

	if (this->fElapsedTime > this->fDuration)
	{
		fElapsedTime = 0;
		if (this->iRepeatRequired != -1 && this->iRepeatCount >= this->iRepeatRequired) // if it has completed the amount of loops required 
			bIsDone = true;
		else
		{
			this->iRepeatCount++;
		}
	}
}
