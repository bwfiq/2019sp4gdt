#include "UIReligionBar.h"
#include "SceneData.h"

UIReligionBar::UIReligionBar() :
	UIBase()
{
	SceneData* SD = SceneData::GetInstance();
	UIComponent UIC;
	UIC.mesh = SD->GetMesh("whitequad");
	uiComponents_list.push_back(UIC);
	UIC.mesh = SD->GetMesh("greenquad");
	UIC.scale.Set(0.5f, 0.5f);
	uiComponents_list.push_back(UIC);
}

UIReligionBar::~UIReligionBar()
{
}

void UIReligionBar::Update(float dt)
{
}
