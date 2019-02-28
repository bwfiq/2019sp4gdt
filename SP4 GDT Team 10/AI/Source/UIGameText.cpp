#include "UIGameText.h"
#include "SceneData.h"
#include "PostOffice.h"
#include "ConcreteMessages.h"
#include "GameObject.h"
#include "Building.h"
#include "Villager.h"
#include "Pig.h"
#include "Bush.h"

UIGameText::UIGameText(TEXT_TYPE textType, float x, float y, GameObject* go) :
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
	case TEXT_SELECTED_RLAB:
		pos.Set(1, 0.25f);
		scale.Set(250, 250);
		anchorPoint.Set(1, 0);
		uiComponents_list[COMPONENT_BOARD].mesh = SD->GetMesh("ui_board_blank");
		uiComponents_list[COMPONENT_TEXT_1].text = "Research Lab";
		for (int i = 0; i <= COMPONENT_TEXT_5 - COMPONENT_TEXT_1; ++i)
		{
			uiComponents_list[COMPONENT_TEXT_1 + i].text = "";
			uiComponents_list[COMPONENT_TEXT_1 + i].textSize = scale.y * 0.1f;
		}
		uiComponents_list[COMPONENT_TEXT_5].text = "Research Lab";
		uiComponents_list[COMPONENT_TEXT_5].textSize *= 1.1f;
		uiComponents_list[COMPONENT_TEXT_5].pos.x = 0.1f;

		uiComponents_list[COMPONENT_TEXT_4].text = "Research Points : ";
		uiComponents_list[COMPONENT_TEXT_4].textSize *= 0.85f;
		uiComponents_list[COMPONENT_TEXT_4].pos.x = 0.1f;

		uiComponents_list[COMPONENT_TEXT_3].text = "aa";
		uiComponents_list[COMPONENT_TEXT_3].textSize *= 0.9f;
		uiComponents_list[COMPONENT_TEXT_3].pos.x = 0.275f;

		uiComponents_list[COMPONENT_TEXT_2].text = "Next Point :";
		uiComponents_list[COMPONENT_TEXT_2].textSize *= 1.f;
		uiComponents_list[COMPONENT_TEXT_2].pos.x = 0.15f;

		uiComponents_list[COMPONENT_TEXT_1].text = "asdasd";
		uiComponents_list[COMPONENT_TEXT_1].textSize *= 1.15f;
		uiComponents_list[COMPONENT_TEXT_1].pos.x = 0.25f;
		break;
	case TEXT_SELECTED_BUILDING:
	{
		Building* building = dynamic_cast<Building*>(go);
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
		uiComponents_list[COMPONENT_TEXT_4].text = "v Info v";
		uiComponents_list[COMPONENT_TEXT_4].pos.x += 0.04f;
		if (go->type == GameObject::GO_CHIEFHUT)
		{
			uiComponents_list[COMPONENT_TEXT_5].text = "Chief Hut";
			uiComponents_list[COMPONENT_TEXT_3].textSize *= 0.65f;
			uiComponents_list[COMPONENT_TEXT_3].text = "Makes blueprints that";
			uiComponents_list[COMPONENT_TEXT_3].pos.x -= 0.105f;
			uiComponents_list[COMPONENT_TEXT_2].textSize *= 0.6f;
			uiComponents_list[COMPONENT_TEXT_2].text = "Villagers can build with";
			uiComponents_list[COMPONENT_TEXT_2].pos.x -= 0.095f;
		}
		else if (go->type == GameObject::GO_HOUSE)
		{
			uiComponents_list[COMPONENT_TEXT_5].text = "House";
			uiComponents_list[COMPONENT_TEXT_5].pos.x += 0.09f;
			uiComponents_list[COMPONENT_TEXT_3].textSize *= 0.75f;
			uiComponents_list[COMPONENT_TEXT_3].text = "Increases population";
			uiComponents_list[COMPONENT_TEXT_3].pos.x -= 0.105f;
			uiComponents_list[COMPONENT_TEXT_2].textSize *= 0.75f;
			uiComponents_list[COMPONENT_TEXT_2].text = "limit";
			uiComponents_list[COMPONENT_TEXT_2].pos.x += 0.095f;
		}
		else if (go->type == GameObject::GO_LOGS)
		{
			uiComponents_list[COMPONENT_TEXT_5].text = "Logs";
			uiComponents_list[COMPONENT_TEXT_5].pos.x += 0.12f;
			uiComponents_list[COMPONENT_TEXT_3].textSize *= 0.75f;
			uiComponents_list[COMPONENT_TEXT_3].text = "Reduces power of";
			uiComponents_list[COMPONENT_TEXT_3].pos.x -= 0.075f;
			uiComponents_list[COMPONENT_TEXT_2].textSize *= 0.75f;
			uiComponents_list[COMPONENT_TEXT_2].text = "Calamity objects";
			uiComponents_list[COMPONENT_TEXT_2].pos.x -= 0.055f;
		}
		else if (go->type == GameObject::GO_GRANARY)
		{
			uiComponents_list[COMPONENT_TEXT_5].text = "Granary";
			uiComponents_list[COMPONENT_TEXT_5].pos.x += 0.03f;
			uiComponents_list[COMPONENT_TEXT_3].textSize *= 0.85f;
			uiComponents_list[COMPONENT_TEXT_3].text = "Increases food";
			uiComponents_list[COMPONENT_TEXT_3].pos.x -= 0.055f;
			uiComponents_list[COMPONENT_TEXT_2].textSize *= 0.85f;
			uiComponents_list[COMPONENT_TEXT_2].text = "limit";
			uiComponents_list[COMPONENT_TEXT_2].pos.x += 0.095f;
		}
		else if (go->type == GameObject::GO_WOODSHED)
		{
			uiComponents_list[COMPONENT_TEXT_5].text = "Woodshed";
			uiComponents_list[COMPONENT_TEXT_3].textSize *= 0.85f;
			uiComponents_list[COMPONENT_TEXT_3].text = "Increases wood";
			uiComponents_list[COMPONENT_TEXT_3].pos.x -= 0.055f;
			uiComponents_list[COMPONENT_TEXT_2].textSize *= 0.85f;
			uiComponents_list[COMPONENT_TEXT_2].text = "limit";
			uiComponents_list[COMPONENT_TEXT_2].pos.x += 0.095f;
		}
		else if (go->type == GameObject::GO_STONESHED)
		{
			uiComponents_list[COMPONENT_TEXT_5].text = "Stoneshed";
			uiComponents_list[COMPONENT_TEXT_3].textSize *= 0.85f;
			uiComponents_list[COMPONENT_TEXT_3].text = "Increases stone";
			uiComponents_list[COMPONENT_TEXT_3].pos.x -= 0.055f;
			uiComponents_list[COMPONENT_TEXT_2].textSize *= 0.85f;
			uiComponents_list[COMPONENT_TEXT_2].text = "limit";
			uiComponents_list[COMPONENT_TEXT_2].pos.x += 0.095f;
		}
		else if (go->type == GameObject::GO_RESEARCHLAB)
		{
			uiComponents_list[COMPONENT_TEXT_5].text = "Research Lab";
			uiComponents_list[COMPONENT_TEXT_5].pos.x -= 0.1f;
			uiComponents_list[COMPONENT_TEXT_3].textSize *= 0.85f;
			uiComponents_list[COMPONENT_TEXT_3].text = "Unlocks building";
			uiComponents_list[COMPONENT_TEXT_3].pos.x -= 0.055f;
			uiComponents_list[COMPONENT_TEXT_2].textSize *= 0.85f;
			uiComponents_list[COMPONENT_TEXT_2].text = "tiers";
			uiComponents_list[COMPONENT_TEXT_2].pos.x += 0.095f;
		}
		switch (building->eCurrState)
		{
		case Building::COMPLETED:
			uiComponents_list[COMPONENT_TEXT_1].text = "= Completed =";
			uiComponents_list[COMPONENT_TEXT_1].textColor.Set(0, 1, 0);
			uiComponents_list[COMPONENT_TEXT_1].pos.x -= 0.08f;
			break;
		case Building::CONSTRUCTING:
			uiComponents_list[COMPONENT_TEXT_1].text = "= Unconstructed =";
			uiComponents_list[COMPONENT_TEXT_1].textColor.Set(0.73f, 0.44f, 0);
			uiComponents_list[COMPONENT_TEXT_1].pos.x -= 0.17f;
			uiComponents_list[COMPONENT_TEXT_4].text = "Requires construction!";
			uiComponents_list[COMPONENT_TEXT_4].textSize *= 0.7f;
			uiComponents_list[COMPONENT_TEXT_4].pos.x -= 0.1f;
			uiComponents_list[COMPONENT_TEXT_4].textColor.Set(0.2, 0.2f, 0.2f);
			break;
		case Building::BROKEN:
			uiComponents_list[COMPONENT_TEXT_1].text = "= Broken =";
			uiComponents_list[COMPONENT_TEXT_1].textColor.Set(1, 0, 0);
			uiComponents_list[COMPONENT_TEXT_1].pos.x -= 0.04f;
			uiComponents_list[COMPONENT_TEXT_4].text = "Requires repair!";
			uiComponents_list[COMPONENT_TEXT_4].textSize *= 0.9f;
			uiComponents_list[COMPONENT_TEXT_4].pos.x -= 0.1f;
			uiComponents_list[COMPONENT_TEXT_4].textColor.Set(1, 0.2f, 0.2f);
			break;
		case Building::BLUEPRINT:
			uiComponents_list[COMPONENT_TEXT_1].text = "= Blueprint =";
			uiComponents_list[COMPONENT_TEXT_1].textColor.Set(0, 0.69f, 0.73f);
			uiComponents_list[COMPONENT_TEXT_1].pos.x -= 0.08f;
			break;
		}
	}
		
		break;
	case TEXT_SELECTED_VILLAGER:
	{
		Villager* goVil = dynamic_cast<Villager*>(go);
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
		//uiComponents_list[COMPONENT_TEXT_5].pos.x += 0.02f;
		//villager fsm state, enum state(healthy, etc), enum stat(mining, etc)
		uiComponents_list[COMPONENT_TEXT_4].text = "v Info v";
		uiComponents_list[COMPONENT_TEXT_4].pos.x += 0.04f;
		for (int i = 0; i <= COMPONENT_TEXT_3 - COMPONENT_TEXT_1; ++i)
		{
			uiComponents_list[COMPONENT_TEXT_1 + i].textSize *= 0.75f;
			uiComponents_list[COMPONENT_TEXT_1 + i].pos.x -= 0.1f;
		}
		//uiComponents_list[COMPONENT_TEXT_3].text = "Action:";
		//uiComponents_list[COMPONENT_TEXT_2].text = "Stat:";
		uiComponents_list[COMPONENT_TEXT_3].text = "Best at:";
		Villager::STATS bestAt = (Villager::STATS)0;
		for (int i = 0; i < Villager::STAT_TOTAL; ++i)
		{
			if (goVil->fStats[bestAt] < goVil->fStats[i])//if the iterating stat is better than the "current best" stat
			{
				bestAt = (Villager::STATS)i;
			}
		}
		switch (bestAt)
		{
		case Villager::HUNTING:
			uiComponents_list[COMPONENT_TEXT_3].text += "Hunting";
			break;
		case Villager::FORAGING:
			uiComponents_list[COMPONENT_TEXT_3].text += "Foraging";
			break;
		case Villager::WOODCUTTING:
			uiComponents_list[COMPONENT_TEXT_3].text += "Woodcutting";
			break;
		case Villager::BUILDING:
			uiComponents_list[COMPONENT_TEXT_3].text += "Building";
			break;
		case Villager::BREEDING:
			uiComponents_list[COMPONENT_TEXT_3].text += "Breeding";
			break;
		case Villager::COMBAT:
			uiComponents_list[COMPONENT_TEXT_3].text += "Combat";
			break;
		case Villager::MINING:
			uiComponents_list[COMPONENT_TEXT_3].text += "Mining";
			break;
		}
		uiComponents_list[COMPONENT_TEXT_1].text = "State: ";
		switch (goVil->eCurrState)
		{
		case Villager::HEALTHY:
			uiComponents_list[COMPONENT_TEXT_1].text += "Healthy";
			break;
		case Villager::PANIC:
			uiComponents_list[COMPONENT_TEXT_1].text += "Panic";
			break;
		case Villager::TIRED:
			uiComponents_list[COMPONENT_TEXT_1].text += "Tired";
			break;
		case Villager::SICKLY:
			uiComponents_list[COMPONENT_TEXT_1].text += "Sickly";
			break;
		case Villager::DYING:
			uiComponents_list[COMPONENT_TEXT_1].text += "Dying";
			break;
		}
	}
	break;
	case TEXT_SELECTED_PIG:
	{
		Pig* goPig = dynamic_cast<Pig*>(go);
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
		uiComponents_list[COMPONENT_TEXT_5].text = "Pig";
		//uiComponents_list[COMPONENT_TEXT_5].pos.x += 0.02f;
		uiComponents_list[COMPONENT_TEXT_4].text = "v Info v";
		uiComponents_list[COMPONENT_TEXT_4].pos.x += 0.04f;
		for (int i = 0; i <= COMPONENT_TEXT_3 - COMPONENT_TEXT_1; ++i)
		{
			uiComponents_list[COMPONENT_TEXT_1 + i].textSize *= 0.75f;
			uiComponents_list[COMPONENT_TEXT_1 + i].pos.x -= 0.1f;
		}
		uiComponents_list[COMPONENT_TEXT_3].text = "Chonkness: " + std::to_string(goPig->iFoodAmount);
		//uiComponents_list[COMPONENT_TEXT_2].text = "Stat:";
		uiComponents_list[COMPONENT_TEXT_1].text = "State: ";
		switch (goPig->state)
		{
		case Pig::WILD:
			uiComponents_list[COMPONENT_TEXT_1].text += "Wild";
			break;
		case Pig::TAME:
			uiComponents_list[COMPONENT_TEXT_1].text += "Tame";
			break;
		}
	}
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
		uiComponents_list[COMPONENT_TEXT_3].pos.x += 0.03f;
		if (go->type == GameObject::GO_BUSH)
		{
			uiComponents_list[COMPONENT_TEXT_5].text = "Bush";
			uiComponents_list[COMPONENT_TEXT_5].pos.x += 0.22f;
			uiComponents_list[COMPONENT_TEXT_3].text = "Food";
			uiComponents_list[COMPONENT_TEXT_3].textColor.Set(99.f / 255.f, 170.f / 255.f, 71.f / 255.f);
			uiComponents_list[COMPONENT_TEXT_2].textSize *= 0.75f;
			uiComponents_list[COMPONENT_TEXT_2].text = "State: ";

			Bush* goBush = dynamic_cast<Bush*>(go);
			switch (goBush->eCurrState)
			{
			case Bush::LUSH:
				uiComponents_list[COMPONENT_TEXT_2].text += "Lush";
				break;
			case Bush::DEPLETED:
				uiComponents_list[COMPONENT_TEXT_2].text += "Depleted";
				break;
			}
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
		uiComponents_list[COMPONENT_TEXT_5].pos.x -= 0.1f;
		uiComponents_list[COMPONENT_TEXT_4].pos.x -= 0.04f;
		uiComponents_list[COMPONENT_TEXT_3].pos.x += 0.08f;
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
	UIBase::UpdateTween(dt);
}

void UIGameText::SetText(std::string text)
{
}
