#include "UIAltarPopup.h"
#include "SceneData.h"
#include "Application.h"
#include "MouseController.h"

UIAltarPopup::UIAltarPopup() :
	UIBase()
{
	SceneData* SD = SceneData::GetInstance();
	uiComponents_list.resize(COMPONENT_TOTAL);
	uiComponents_list[COMPONENT_OUTLINEBAR].mesh = SD->GetMesh("whitequad");
	uiComponents_list[COMPONENT_GREYBAR].mesh = SD->GetMesh("greyquad");

	pos.Set(0.9f, 0);
	scale.Set(Application::GetInstance().GetWindowWidth() * 0.1f * 1.5f, Application::GetInstance().GetWindowWidth() * 0.1f);
	anchorPoint.Set(1, 0);
	float ratio = scale.x / scale.y;

	uiComponents_list[COMPONENT_OUTLINEBAR].scale.Set(1 + 0.2f / ratio, 1.2f);
	uiComponents_list[COMPONENT_OUTLINEBAR].anchorPoint.Set(0, 0);

	uiComponents_list[COMPONENT_GREYBAR].pos.Set(0, 0);
	uiComponents_list[COMPONENT_GREYBAR].anchorPoint.Set(0, 0);
	uiComponents_list[COMPONENT_GREYBAR].scale.Set(1, 1);
	uiComponents_list[COMPONENT_GREYBAR].text = "HELLO";
	uiComponents_list[COMPONENT_GREYBAR].textSize = scale.y;

	bCursorVisibleThing = false;
}

UIAltarPopup::~UIAltarPopup()
{
}

void UIAltarPopup::Update(float dt)
{
	MouseController* MC = MouseController::GetInstance();
	if (MC->IsMouseOnUI() && IsMouseHovered())
	{

	}
	if (scale != Vector3(Application::GetInstance().GetWindowWidth() * 0.1f * 1.5f, Application::GetInstance().GetWindowWidth() * 0.1f, scale.z))
	{
		scale.Set(Application::GetInstance().GetWindowWidth() * 0.1f * 1.5f, Application::GetInstance().GetWindowWidth() * 0.1f);
		float ratio = scale.x / scale.y;
		uiComponents_list[COMPONENT_OUTLINEBAR].scale.Set(1 + 0.2f / ratio, 1.2f);
		uiComponents_list[COMPONENT_GREYBAR].textSize = scale.y;
	}
}
