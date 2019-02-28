#include "UIMessagePopup.h"
#include "SceneData.h"
#include "UITween.h"

UIMessagePopup::UIMessagePopup(const std::string& text, float duration) :
	UIBase()
{
	SceneData* SD = SceneData::GetInstance();
	uiComponents_list.resize(COMPONENT_TOTAL);
	uiComponents_list[COMPONENT_OUTLINEBAR].mesh = SD->GetMesh("redquad");
	uiComponents_list[COMPONENT_GREYBAR].mesh = SD->GetMesh("greyquad");

	//pos.Set(0.5f, 0.5f);
	pos.Set(1.5f, 0.5f);
	scale.Set(600, 60);
	anchorPoint.Set(0.5, 0.5);
	float ratio = scale.x / scale.y;

	uiComponents_list[COMPONENT_OUTLINEBAR].scale.Set(1 + 0.2f / ratio, 1.2f);
	uiComponents_list[COMPONENT_OUTLINEBAR].anchorPoint.Set(0, 0);

	uiComponents_list[COMPONENT_GREYBAR].pos.Set(0, 0);
	uiComponents_list[COMPONENT_GREYBAR].anchorPoint.Set(0, 0);
	uiComponents_list[COMPONENT_GREYBAR].scale.Set(1, 1);
	uiComponents_list[COMPONENT_TEXT].text = text;
	uiComponents_list[COMPONENT_TEXT].textSize = scale.y * 0.75f;
	uiComponents_list[COMPONENT_TEXT].pos.Set(0.f, 0.5f);

	UITween* newTween = new UITween(
		this
		, 0.5f
		, UITween::ES_SINE
		, UITween::ED_OUT
	);
	newTween->properties_goal["pos"].Set(0.5f, 0.5f);
	this->AddTween(newTween);

	this->fDuration = duration;
}

UIMessagePopup::~UIMessagePopup()
{
}

void UIMessagePopup::Update(float dt)
{
	UIBase::UpdateTween(dt);
	SceneData* SD = SceneData::GetInstance();
	fElapsedTime += dt;
	if (fmodf(fElapsedTime, 0.3f) < 0.15f)
		uiComponents_list[COMPONENT_OUTLINEBAR].mesh = SD->GetMesh("redquad");
	else
		uiComponents_list[COMPONENT_OUTLINEBAR].mesh = SD->GetMesh("greenquad");
	if (fElapsedTime > fDuration)
	{
		UITween* newTween = new UITween(
			this
			, 0.5f
			, UITween::ES_SINE
			, UITween::ED_IN
			, 0
			, 0
			, UITween::CALLBACK_SET_UI_DONE
		);
		newTween->properties_goal["pos"].Set(-0.5f, 0.5f);
		this->AddTween(newTween);
		//bIsDone = true;
		//CSoundEngine::GetInstance()->StopASound("rumble");
	}
}
