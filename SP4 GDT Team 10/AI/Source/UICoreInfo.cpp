#include "UICoreInfo.h"
#include "SceneData.h"

UICoreInfo::UICoreInfo(INFO_TYPE infoType, Vector3 origPos) :
	UIBase()
{
	SceneData* SD = SceneData::GetInstance();

	uiComponents_list.resize(COMPONENT_TOTAL);
	switch (infoType)
	{
	case INFO_DAY:
		uiComponents_list[COMPONENT_BOARD].mesh = SD->GetMesh("ui_board_day");
		break;
	case INFO_FOOD:
		uiComponents_list[COMPONENT_BOARD].mesh = SD->GetMesh("ui_board_food");
		break;
	case INFO_POPULATION:
		uiComponents_list[COMPONENT_BOARD].mesh = SD->GetMesh("ui_board_population");
		break;
	case INFO_STONE:
		uiComponents_list[COMPONENT_BOARD].mesh = SD->GetMesh("ui_board_stone");
		break;
	case INFO_TIME:
		uiComponents_list[COMPONENT_BOARD].mesh = SD->GetMesh("ui_board_time");
		break;
	case INFO_WOOD:
		uiComponents_list[COMPONENT_BOARD].mesh = SD->GetMesh("ui_board_wood");
		break;
	}
	this->infoType = infoType;
	this->origPos = origPos;

	pos.Set(origPos.x, origPos.y);
	//scale.Set(170, 50);
	scale.Set(Application::GetInstance().GetWindowWidth() * 0.17f, Application::GetInstance().GetWindowWidth() * 0.05f);
	anchorPoint.Set(0.5f, 0.5f);
	float ratio = scale.x / scale.y;

	uiComponents_list[COMPONENT_BOARD].scale.Set(1, 1);
	uiComponents_list[COMPONENT_BOARD].anchorPoint.Set(0, 0);

	uiComponents_list[COMPONENT_TEXT].mesh = NULL;
	uiComponents_list[COMPONENT_TEXT].text = "asd";
	uiComponents_list[COMPONENT_TEXT].textSize = scale.y * 0.4f;
	uiComponents_list[COMPONENT_TEXT].pos.Set(0.45f, 0.5);
	uiComponents_list[COMPONENT_TEXT].anchorPoint.Set(0, 0);

}

UICoreInfo::~UICoreInfo()
{
}

void UICoreInfo::Update(float dt)
{
	SceneData* SD = SceneData::GetInstance();
	switch (infoType)
	{
	case INFO_DAY:
		uiComponents_list[COMPONENT_TEXT].text = SD->GetCurrMonth_string() + " " + std::to_string(SD->GetCurrDay());
		break;
	case INFO_FOOD:
		uiComponents_list[COMPONENT_TEXT].text = std::to_string(SD->GetFood()) + "/" + std::to_string(SD->GetFoodLimit());
		break;
	case INFO_POPULATION:
		uiComponents_list[COMPONENT_TEXT].text = std::to_string(SD->GetPopulation()) + "/" + std::to_string(SD->GetPopulationLimit());
		break;
	case INFO_STONE:
		uiComponents_list[COMPONENT_TEXT].text = std::to_string(SD->GetStone()) + "/" + std::to_string(SD->GetStoneLimit());
		break;
	case INFO_TIME:
		uiComponents_list[COMPONENT_TEXT].text = SD->GetTimeOfDay_string();
		break;
	case INFO_WOOD:
		uiComponents_list[COMPONENT_TEXT].text = std::to_string(SD->GetWood()) + "/" + std::to_string(SD->GetWoodLimit());
		break;
	}
	if (scale != Vector3(Application::GetInstance().GetWindowWidth() * 0.17f, Application::GetInstance().GetWindowWidth() * 0.05f, scale.z))
	{
		scale.Set(Application::GetInstance().GetWindowWidth() * 0.17f, Application::GetInstance().GetWindowWidth() * 0.05f);
		float ratio = scale.x / scale.y;
		uiComponents_list[COMPONENT_BOARD].scale.Set(1, 1);
		uiComponents_list[COMPONENT_TEXT].textSize = scale.y * 0.4f;
	}
}
