#include "UICoreResources.h"
#include "SceneData.h"

UICoreResources::UICoreResources() :
	UIBase()
{
	SceneData* SD = SceneData::GetInstance();

	uiComponents_list.resize(COMPONENT_TOTAL);
	uiComponents_list[COMPONENT_OUTLINEBAR].mesh = SD->GetMesh("whitequad");

	pos.Set(0.5f, 0.9f);
	scale.Set(600, 40);
	anchorPoint.Set(0.5, 0.5);
	float ratio = scale.x / scale.y;
	
	uiComponents_list[COMPONENT_OUTLINEBAR].scale.Set(1 + 0.2f / ratio, 1.2f);
	uiComponents_list[COMPONENT_OUTLINEBAR].anchorPoint.Set(0, 0);
}

UICoreResources::~UICoreResources()
{
}

void UICoreResources::Update(float dt)
{
	SceneData* SD = SceneData::GetInstance();
}
