#include "UIReligionBar.h"
#include "SceneData.h"

UIReligionBar::UIReligionBar() :
	UIBase()
{
	SceneData* SD = SceneData::GetInstance();
	UIComponent UIC;
	UIC.mesh = SD->GetMesh("whitequad");
	uiComponents_list.push_back(UIC);
	UIC.mesh = SD->GetMesh("redquad");
	uiComponents_list.push_back(UIC);
	UIC.mesh = SD->GetMesh("greyquad");
	uiComponents_list.push_back(UIC);
	UIC.mesh = SD->GetMesh("greenquad");
	uiComponents_list.push_back(UIC);
	UIC.mesh = SD->GetMesh("blueTriangle");
	uiComponents_list.push_back(UIC);

	pos.Set(0.5f, 0.9f);
	scale.Set(600, 40);
	anchorPoint.Set(0.5, 0.5);
	float ratio = scale.x / scale.y;
	
	uiComponents_list[COMPONENT_OUTLINEBAR].scale.Set(1 + 0.2f / ratio, 1.2f);
	uiComponents_list[COMPONENT_OUTLINEBAR].anchorPoint.Set(0, 0);

	uiComponents_list[COMPONENT_REDBAR].pos.Set(0, 0);
	uiComponents_list[COMPONENT_REDBAR].anchorPoint.Set(0, 0);
	uiComponents_list[COMPONENT_REDBAR].scale.Set(1.f / 3.f, 1);

	uiComponents_list[COMPONENT_GREYBAR].pos.Set(1.f/3.f, 0);
	uiComponents_list[COMPONENT_GREYBAR].anchorPoint.Set(0, 0);
	uiComponents_list[COMPONENT_GREYBAR].scale.Set(1.f / 3.f, 1);

	uiComponents_list[COMPONENT_GREENBAR].pos.Set(2.f/3.f, 0);
	uiComponents_list[COMPONENT_GREENBAR].anchorPoint.Set(0, 0);
	uiComponents_list[COMPONENT_GREENBAR].scale.Set(1.f / 3.f, 1);

	uiComponents_list[COMPONENT_BLUETRIANGLE].pos.Set(0, 0);
	uiComponents_list[COMPONENT_BLUETRIANGLE].scale.Set(0.025f, 0.5f);
	uiComponents_list[COMPONENT_BLUETRIANGLE].anchorPoint.Set(0.5f, 0.5f);
}

UIReligionBar::~UIReligionBar()
{
}

void UIReligionBar::Update(float dt)
{
	float fReligionValue = SceneData::GetInstance()->GetReligionValue();
	uiComponents_list[COMPONENT_BLUETRIANGLE].pos.Set(
		Math::lerp(uiComponents_list[COMPONENT_BLUETRIANGLE].pos.x, fReligionValue / 100.f, Math::Min((float)dt * 10.f,1.f))
		, 0);
	if (this->IsMousePressed())
	{
		std::cout << "a" << std::endl;
	}
}
