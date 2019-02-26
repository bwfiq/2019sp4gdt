#include "UITween.h"
#include "UIBase.h"
#include "EasingStyle.h"

static const Vector3 NO_TWEEN_PLEZ(-1, -1, -1);//basically a kinda way to check if u want to Tween a property or not, if its -1,-1,-1 then u dont wanna tween it

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
	fElapsedTime -= delayTime;//if ders a delay time den il just subtract some elapsedtime
	properties_start["pos"] = ui->pos;
	properties_start["scale"] = ui->scale;
	properties_goal["pos"] = NO_TWEEN_PLEZ;
	properties_goal["scale"] = NO_TWEEN_PLEZ;
	switch (easingStyle)
	{
	case ES_BACK:
		easingFunction = EasingStyle::easeOutBack;
	}
}

UITween::~UITween()
{
}

bool UITween::IsDone()
{
	return this->bIsDone;
}

void UITween::Update(float dt)
{
	if (IsDone()) return;
	fElapsedTime += dt;
	if (fElapsedTime > 0)
	{
		float alpha_linear = Math::Min(fElapsedTime / fDuration, 1.f);
		float alpha_eased = alpha_linear;
		if (this->easingStyle != ES_LINEAR)
		{
			alpha_eased = this->easingFunction(alpha_linear, 0, 1, 1);
		}
		if (properties_goal["pos"] != NO_TWEEN_PLEZ)//probably could be optimised
		{
			ui->pos = properties_start["pos"].lerped(properties_goal["pos"], alpha_eased);
		}
		if (properties_goal["scale"] != NO_TWEEN_PLEZ)//probably could be optimised
		{
			ui->scale = properties_start["scale"].lerped(properties_goal["scale"], alpha_eased);
		}
	}

	if (this->fElapsedTime > this->fDuration)
	{
		fElapsedTime = 0;
		if (this->iRepeatRequired != -1 && this->iRepeatCount >= this->iRepeatRequired)//if it has completed the amount of loops required 
			bIsDone = true;
		else
		{
			this->iRepeatCount++;
		}
	}
}
