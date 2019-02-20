#include "UIOverlay.h"
#include "SceneData.h"

UIOverlay::UIOverlay(const std::string& text, float x, float y) :
	UIBase()
{
	SceneData* SD = SceneData::GetInstance();
	uiComponents_list.resize(COMPONENT_TOTAL);
	uiComponents_list[COMPONENT_OVERLAY].mesh = SD->GetMesh("whitequad");

	pos.Set(x, y);
	scale.Set(Application::GetInstance().GetWindowWidth() * 0.7f, Application::GetInstance().GetWindowHeight() * 0.7f);
	anchorPoint.Set(0.5, 0.5);
	float ratio = scale.x / scale.y;

	uiComponents_list[COMPONENT_OVERLAY].pos.Set(0, 0);
	uiComponents_list[COMPONENT_OVERLAY].anchorPoint.Set(0, 0);
	uiComponents_list[COMPONENT_OVERLAY].scale.Set(1, 1);
	uiComponents_list[COMPONENT_OVERLAY].text = text;
	uiComponents_list[COMPONENT_OVERLAY].textSize = scale.y;
	uiComponents_list[COMPONENT_OVERLAY].alpha = 0.5f;
}

UIOverlay::~UIOverlay()
{
}

void UIOverlay::Update(float dt)
{
	if (scale != Vector3(Application::GetInstance().GetWindowWidth() * 0.7f, Application::GetInstance().GetWindowHeight() * 0.7f, scale.z))
	{
		scale.Set(Application::GetInstance().GetWindowWidth() * 0.7f, Application::GetInstance().GetWindowHeight() * 0.7f);
		float ratio = scale.x / scale.y;
		uiComponents_list[COMPONENT_OVERLAY].textSize = scale.y;
	}
}
