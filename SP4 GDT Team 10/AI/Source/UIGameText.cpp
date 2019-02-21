#include "UIGameText.h"
#include "SceneData.h"
#include "PostOffice.h"
#include "ConcreteMessages.h"
#include "GameObject.h"

UIGameText::UIGameText(TEXT_TYPE textType, GameObject* go) :
	UIBase()
{
	SceneData* SD = SceneData::GetInstance();
	uiComponents_list.resize(COMPONENT_TOTAL);
	for (int i = 0; i <= COMPONENT_TEXT_5 - COMPONENT_TEXT_1; ++i)
	{
		uiComponents_list[COMPONENT_TEXT_1 + i].mesh = NULL;
		uiComponents_list[COMPONENT_TEXT_1 + i].pos.Set(0.2f, ((i / 6.f) + 0.15f));
		uiComponents_list[COMPONENT_TEXT_1 + i].scale.Set(1, 0.2f);
		uiComponents_list[COMPONENT_TEXT_1 + i].anchorPoint.Set(0, 0);
	}
	this->go = go;
	switch (textType)
	{
	case TEXT_DAILYREQUIREMENT:
		pos.Set(0, 0.6);
		scale.Set(Application::GetInstance().GetWindowWidth() * 0.2f, Application::GetInstance().GetWindowWidth() * 0.2f * 1.1);
		anchorPoint.Set(0, 1);
		uiComponents_list[COMPONENT_BOARD].mesh = SD->GetMesh("ui_board_blank");
		for (int i = 0; i <= COMPONENT_TEXT_5 - COMPONENT_TEXT_1; ++i)
		{
			uiComponents_list[COMPONENT_TEXT_1 + i].text = "";
			uiComponents_list[COMPONENT_TEXT_1 + i].textSize = scale.y * 0.1f;
		}
		break;
	case TEXT_SELECTED_ALTAR:
		pos.Set(1, 0.25f);
		scale.Set(250, 250);
		anchorPoint.Set(1, 0);
		uiComponents_list[COMPONENT_BOARD].mesh = SD->GetMesh("ui_board_blank");
		uiComponents_list[COMPONENT_TEXT_1].text = "Altar";
		for (int i = 0; i <= COMPONENT_TEXT_5 - COMPONENT_TEXT_1; ++i)
		{
			uiComponents_list[COMPONENT_TEXT_1 + i].text = "";
			uiComponents_list[COMPONENT_TEXT_1 + i].textSize = scale.y * 0.1f;
		}
		uiComponents_list[COMPONENT_TEXT_5].text = "Altar";
		uiComponents_list[COMPONENT_TEXT_5].textSize *= 1.3f;
		uiComponents_list[COMPONENT_TEXT_5].pos.x = 0.3f;

		uiComponents_list[COMPONENT_TEXT_4].text = "Appeasement : ";
		uiComponents_list[COMPONENT_TEXT_4].textSize *= 0.85f;
		uiComponents_list[COMPONENT_TEXT_4].pos.x = 0.1f;

		uiComponents_list[COMPONENT_TEXT_3].text = "aa";
		uiComponents_list[COMPONENT_TEXT_3].textSize *= 0.9f;
		uiComponents_list[COMPONENT_TEXT_3].pos.x = 0.275f;

		uiComponents_list[COMPONENT_TEXT_2].text = "The gods are :";
		uiComponents_list[COMPONENT_TEXT_2].textSize *= 1.f;
		uiComponents_list[COMPONENT_TEXT_2].pos.x = 0.15f;

		uiComponents_list[COMPONENT_TEXT_1].text = "asdasd";
		uiComponents_list[COMPONENT_TEXT_1].textSize *= 1.15f;
		uiComponents_list[COMPONENT_TEXT_1].pos.x = 0.25f;
		break;
	case TEXT_SELECTED_BUILDING:
		pos.Set(1, 0.25f);
		scale.Set(250, 250);
		anchorPoint.Set(1, 0);
		uiComponents_list[COMPONENT_BOARD].mesh = SD->GetMesh("ui_board_blank");
		for (int i = 0; i <= COMPONENT_TEXT_5 - COMPONENT_TEXT_1; ++i)
		{
			uiComponents_list[COMPONENT_TEXT_1 + i].text = "";
			uiComponents_list[COMPONENT_TEXT_1 + i].textSize = scale.y * 0.1f;
		}
		uiComponents_list[COMPONENT_TEXT_5].textSize *= 1.3f;
		if (go->type == GameObject::GO_CHIEFHUT)
		{
			uiComponents_list[COMPONENT_TEXT_5].text = "Chief Hut";
			uiComponents_list[COMPONENT_TEXT_5].pos.x += 0.08f;
		}
		else if (go->type == GameObject::GO_HOUSE)
		{
			uiComponents_list[COMPONENT_TEXT_5].text = "House";
			uiComponents_list[COMPONENT_TEXT_5].pos.x += 0.2f;
		}

		break;
	case TEXT_SELECTED_VILLAGER:
		pos.Set(1, 0.25f);
		scale.Set(250, 250);
		anchorPoint.Set(1, 0);
		uiComponents_list[COMPONENT_BOARD].mesh = SD->GetMesh("ui_board_blank");
		for (int i = 0; i <= COMPONENT_TEXT_5 - COMPONENT_TEXT_1; ++i)
		{
			uiComponents_list[COMPONENT_TEXT_1 + i].text = "";
			uiComponents_list[COMPONENT_TEXT_1 + i].textSize = scale.y * 0.1f;
		}
		uiComponents_list[COMPONENT_TEXT_5].textSize *= 1.3f;
		uiComponents_list[COMPONENT_TEXT_5].text = "Villager";
		uiComponents_list[COMPONENT_TEXT_5].pos.x += 0.08f;
		break;
	case TEXT_SELECTED_ENVIRONMENT:
		pos.Set(1, 0.25f);
		scale.Set(250, 250);
		anchorPoint.Set(1, 0);
		uiComponents_list[COMPONENT_BOARD].mesh = SD->GetMesh("ui_board_blank");
		for (int i = 0; i <= COMPONENT_TEXT_5 - COMPONENT_TEXT_1; ++i)
		{
			uiComponents_list[COMPONENT_TEXT_1 + i].text = "";
			uiComponents_list[COMPONENT_TEXT_1 + i].textSize = scale.y * 0.1f;
		}
		uiComponents_list[COMPONENT_TEXT_5].textSize *= 1.3f;
		uiComponents_list[COMPONENT_TEXT_4].text = "Resource Type :";
		uiComponents_list[COMPONENT_TEXT_3].textSize *= 1.15f;
		uiComponents_list[COMPONENT_TEXT_3].pos.x += 0.23f;
		if (go->type == GameObject::GO_BUSH)
		{
			uiComponents_list[COMPONENT_TEXT_5].text = "Bush";
			uiComponents_list[COMPONENT_TEXT_5].pos.x += 0.22f;
			uiComponents_list[COMPONENT_TEXT_3].text = "Food";
			uiComponents_list[COMPONENT_TEXT_3].textColor.Set(99.f / 255.f, 170.f / 255.f, 71.f / 255.f);
		}
		else if (go->type == GameObject::GO_MOUNTAIN)
		{
			uiComponents_list[COMPONENT_TEXT_5].text = "Mountain";
			uiComponents_list[COMPONENT_TEXT_5].pos.x += 0.11f;
			uiComponents_list[COMPONENT_TEXT_3].text = "Stone";
			uiComponents_list[COMPONENT_TEXT_3].textColor.Set(0.3f, 0.3f, 0.3f);
		}
		else if (go->type == GameObject::GO_TREE)
		{
			uiComponents_list[COMPONENT_TEXT_5].text = "Tree";
			uiComponents_list[COMPONENT_TEXT_5].pos.x += 0.22f;
			uiComponents_list[COMPONENT_TEXT_3].text = "Wood";
			uiComponents_list[COMPONENT_TEXT_3].textColor.Set(122.f / 255.f, 73.f / 255.f, 9.f / 255.f);
		}
		break;
	}
	this->textType = textType;
	
	float ratio = scale.x / scale.y;
}

UIGameText::~UIGameText()
{
}

void UIGameText::Update(float dt)
{
	SceneData* SD = SceneData::GetInstance();
	switch (this->textType)
	{
	case TEXT_SELECTED_ALTAR:
	{
		float alpha = SD->GetReligionValue() / SD->GetMaxReligionValue();
		uiComponents_list[COMPONENT_TEXT_3].text = std::to_string((int)SD->GetReligionValue()) + " / " + std::to_string((int)SD->GetMaxReligionValue());
		if (alpha < 0.333f)
		{
			uiComponents_list[COMPONENT_TEXT_1].text = "Furious";
			uiComponents_list[COMPONENT_TEXT_1].textColor.Set(0.8f, 0, 0);
		}
		else if (alpha < 0.666f)
		{
			uiComponents_list[COMPONENT_TEXT_1].text = "Neutral";
			uiComponents_list[COMPONENT_TEXT_1].textColor.Set(0.8f, 0.8f, 0);
		}
		else
		{
			uiComponents_list[COMPONENT_TEXT_1].text = "Appeased";
			uiComponents_list[COMPONENT_TEXT_1].textColor.Set(0, 0.8f, 0);
		}
	}
		break;
	}
}

void UIGameText::SetText(std::string text)
{
}
