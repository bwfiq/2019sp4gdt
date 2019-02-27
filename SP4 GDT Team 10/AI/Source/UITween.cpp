#include "UITween.h"
#include "UIBase.h"
#include "EasingStyle.h"

static const Vector3 DISABLE_TWEEN(-1, -1, -1); // if left alone, will not tween; default value for goal
static std::map<std::string, float(*)(float, float, float, float)> easingFunctionMap{
	{"easeOutBack",EasingStyle::easeOutBack}
	,{"easeInBack",EasingStyle::easeInBack}
	,{"easeInOutBack",EasingStyle::easeInOutBack}

	,{"easeOutSine",EasingStyle::easeOutSine}
	,{"easeInSine",EasingStyle::easeInSine}
	,{"easeInOutSine",EasingStyle::easeInOutSine}

	,{"easeOutCubic",EasingStyle::easeOutCubic}
	,{"easeInCubic",EasingStyle::easeInCubic}
	,{"easeInOutCubic",EasingStyle::easeInOutCubic}
};

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! [YOU NEED TO CALL UIBASE'S < UpdateTween > FUNCTION MEMBER FOR TWEENS TO WORK] !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


UITween::UITween(UIBase* ui, float duration, TWEEN_EASINGSTYLE easingStyle, TWEEN_EASINGDIRECTION easingDirection, int repeatCount, /*bool reverses,*/ float delayTime, TWEEN_CALLBACK callBack) :
	fDuration(duration)
	, easingStyle(easingStyle)
	, easingDirection(easingDirection)
	, iRepeatCount(0)
	, iRepeatRequired(repeatCount)
	//, bReverses(reverses)
	, bIsDone(false)
	, ui(ui)
	, callBack(callBack)
{
	fElapsedTime -= delayTime; // adds delay equal to delayTime before animation
	properties_start["pos"] = ui->pos;
	properties_start["scale"] = ui->scale;
	properties_goal["pos"] = DISABLE_TWEEN;
	properties_goal["scale"] = DISABLE_TWEEN;
	/*switch (easingStyle)
	{
	case ES_BACK:
		if (easingDirection == ED_OUT)
			easingFunction = EasingStyle::easeOutBack;
		else if (easingDirection == ED_IN)
			easingFunction = EasingStyle::easeInBack;
		else if (easingDirection == ED_INOUT)
			easingFunction = EasingStyle::easeInOutBack;
	break;
	case ES_CUBIC:
		if (easingDirection == ED_OUT)
			easingFunction = EasingStyle::easeOutCubic;
		else if (easingDirection == ED_IN)
			easingFunction = EasingStyle::easeInCubic;
		else if (easingDirection == ED_INOUT)
			easingFunction = EasingStyle::easeInOutCubic;
	break;
	case ES_SINE:
		if (easingDirection == ED_OUT)
			easingFunction = EasingStyle::easeOutSine;
		else if (easingDirection == ED_IN)
			easingFunction = EasingStyle::easeInSine;
		else if (easingDirection == ED_INOUT)
			easingFunction = EasingStyle::easeInOutSine;
	break;
	}*/

	std::string easingStyleKey = "ease";
	switch (easingDirection)
	{
	case ED_IN:
		easingStyleKey += "In";
		break;
	case ED_OUT:
		easingStyleKey += "Out";
		break;
	case ED_INOUT:
		easingStyleKey += "InOut";
		break;
	}
	switch (easingStyle)
	{
	case ES_BACK:
		easingStyleKey += "Back";
		break;
	case ES_CUBIC:
		easingStyleKey += "Cubic";
		break;
	case ES_SINE:
		easingStyleKey += "Sine";
		break;
	}
	this->easingFunction = easingFunctionMap[easingStyleKey];
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
		this->TweenCallback();
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
		{
			bIsDone = true;
			this->TweenCallback();
		}
		else
		{
			this->iRepeatCount++;
		}
	}
}

void UITween::TweenCallback()
{
	switch (this->callBack)
	{
	case CALLBACK_SET_UI_DONE:
		this->ui->bIsDone = true;
		break;
	}
}
