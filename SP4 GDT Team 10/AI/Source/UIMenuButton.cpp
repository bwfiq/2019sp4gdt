#include "UIMenuButton.h"
#include "SceneData.h"

UIMenuButton::UIMenuButton(const std::string& text) :
	UIBase()
{
	SceneData* SD = SceneData::GetInstance();
	uiComponents_list.resize(COMPONENT_TOTAL);
	uiComponents_list[COMPONENT_OUTLINEBAR].mesh = SD->GetMesh("whitequad");
	uiComponents_list[COMPONENT_GREYBAR].mesh = SD->GetMesh("greyquad");

	pos.Set(0.8f, 0.7f);
	scale.Set(100, 40);
	anchorPoint.Set(0.5, 0.5);
	float ratio = scale.x / scale.y;

	uiComponents_list[COMPONENT_OUTLINEBAR].scale.Set(1 + 0.2f / ratio, 1.2f);
	uiComponents_list[COMPONENT_OUTLINEBAR].anchorPoint.Set(0, 0);

	uiComponents_list[COMPONENT_GREYBAR].pos.Set(0, 0);
	uiComponents_list[COMPONENT_GREYBAR].anchorPoint.Set(0, 0);
	uiComponents_list[COMPONENT_GREYBAR].scale.Set(1, 1);
	uiComponents_list[COMPONENT_GREYBAR].text = text;
	uiComponents_list[COMPONENT_GREYBAR].textSize = scale.y;
}

UIMenuButton::~UIMenuButton()
{
}

void UIMenuButton::Update(float dt)
{
}
