#include "UIMenuButton.h"
#include "SceneData.h"

UIMenuButton::UIMenuButton(const std::string& text, float x, float y) :
	UIBase()
{
	SceneData* SD = SceneData::GetInstance();
	uiComponents_list.resize(COMPONENT_TOTAL);
	uiComponents_list[COMPONENT_OUTLINEBAR].mesh = SD->GetMesh("whitequad");
	uiComponents_list[COMPONENT_GREYBAR].mesh = SD->GetMesh("greyquad");

	pos.Set(x, y);
	scale.Set(Application::GetInstance().GetWindowWidth() * 0.1f, Application::GetInstance().GetWindowWidth()* 0.1f* 0.4f);
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
	if (scale != Vector3(Application::GetInstance().GetWindowWidth() * 0.1f, Application::GetInstance().GetWindowWidth()* 0.1f* 0.4f, scale.z))
	{
		scale.Set(Application::GetInstance().GetWindowWidth() * 0.1f, Application::GetInstance().GetWindowWidth()* 0.1f* 0.4f);
		float ratio = scale.x / scale.y;
		uiComponents_list[COMPONENT_OUTLINEBAR].scale.Set(1 + 0.2f / ratio, 1.2f);
		uiComponents_list[COMPONENT_GREYBAR].textSize = scale.y;
	}
}
