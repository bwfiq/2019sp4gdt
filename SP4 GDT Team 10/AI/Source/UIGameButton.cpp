#include "UIGameButton.h"
#include "SceneData.h"
#include "PostOffice.h"
#include "ConcreteMessages.h"
#include "GameObject.h"

UIGameButton::UIGameButton(BUTTON_TYPE buttonType, float x, float y, unsigned order, GameObject* go) :
	UIBase()
{
	SceneData* SD = SceneData::GetInstance();
	uiComponents_list.resize(COMPONENT_TOTAL);
	switch (buttonType) 
	{
	case BUTTON_DAILYREQUIREMENT:
		pos.Set(0, 0.6f);
		scale.Set(175, 50);
		anchorPoint.Set(0, 0);
		uiComponents_list[COMPONENT_BOARD].mesh = SD->GetMesh("ui_board_blank");
		uiComponents_list[COMPONENT_TEXT].mesh = NULL;
		uiComponents_list[COMPONENT_TEXT].text = "Current Goal v";
		uiComponents_list[COMPONENT_TEXT].textSize = scale.y * 0.4f;
		uiComponents_list[COMPONENT_TEXT].pos.Set(0.1f, 0.5f);
		uiComponents_list[COMPONENT_TEXT].anchorPoint.Set(0, 0);
		break;
	case BUTTON_SELECTED_ALTAR_OFFER:
		scale.Set(250, 50);
		pos.Set(1, 0.25f - (scale.y / (float)Application::GetInstance().GetWindowHeight()) * order);
		anchorPoint.Set(1, 1);
		uiComponents_list[COMPONENT_BOARD].mesh = SD->GetMesh("ui_board_blank");
		uiComponents_list[COMPONENT_TEXT].mesh = NULL;
		uiComponents_list[COMPONENT_TEXT].text = "Offer Food";
		uiComponents_list[COMPONENT_TEXT].textSize = scale.y * 0.5f;
		uiComponents_list[COMPONENT_TEXT].pos.Set(0.2f, 0.5f);
		uiComponents_list[COMPONENT_TEXT].anchorPoint.Set(0, 0);
		break;
	case BUTTON_SELECTED_RLAB:
		scale.Set(250, 50);
		pos.Set(1, 0.25f - (scale.y / (float)Application::GetInstance().GetWindowHeight()) * order);
		anchorPoint.Set(1, 1);
		uiComponents_list[COMPONENT_BOARD].mesh = SD->GetMesh("ui_board_blank");
		uiComponents_list[COMPONENT_TEXT].mesh = NULL;
		uiComponents_list[COMPONENT_TEXT].text = "Open Research";
		uiComponents_list[COMPONENT_TEXT].textSize = scale.y * 0.5f;
		uiComponents_list[COMPONENT_TEXT].pos.Set(0.2f, 0.5f);
		uiComponents_list[COMPONENT_TEXT].anchorPoint.Set(0, 0);
		break;
	case BUTTON_SELECTED_GENERAL_MOVE:
		scale.Set(250, 50);
		pos.Set(1, 0.25f - (scale.y/(float)Application::GetInstance().GetWindowHeight()) * order);
		anchorPoint.Set(1, 1);
		uiComponents_list[COMPONENT_BOARD].mesh = SD->GetMesh("ui_board_blank");
		uiComponents_list[COMPONENT_TEXT].mesh = NULL;
		uiComponents_list[COMPONENT_TEXT].text = "Move";
		uiComponents_list[COMPONENT_TEXT].textSize = scale.y * 0.5f;
		uiComponents_list[COMPONENT_TEXT].pos.Set(0.3f , 0.5f);
		uiComponents_list[COMPONENT_TEXT].anchorPoint.Set(0, 0);
		break;
	case BUTTON_SELECTED_CHIEFHUT_BUILD:
		scale.Set(250, 50);
		pos.Set(1, 0.25f - (scale.y / (float)Application::GetInstance().GetWindowHeight()) * order);
		anchorPoint.Set(1, 1);
		uiComponents_list[COMPONENT_BOARD].mesh = SD->GetMesh("ui_board_blank");
		uiComponents_list[COMPONENT_TEXT].mesh = NULL;
		uiComponents_list[COMPONENT_TEXT].text = "Build";
		uiComponents_list[COMPONENT_TEXT].textSize = scale.y * 0.5f;
		uiComponents_list[COMPONENT_TEXT].pos.Set(0.3f, 0.5f);
		uiComponents_list[COMPONENT_TEXT].anchorPoint.Set(0, 0);
		break;
	case BUTTON_BUILD_LOGS:
	case BUTTON_BUILD_GRANARY:
	case BUTTON_BUILD_HOUSE:
	case BUTTON_BUILD_WOODSHED:
		scale.Set(250, 50);
		pos.Set(1 - (scale.x / (float)Application::GetInstance().GetWindowWidth()), 0.25f + ((scale.y / (float)Application::GetInstance().GetWindowHeight())) * ((int)order - 1));
		anchorPoint.Set(1, 1);
		uiComponents_list[COMPONENT_BOARD].mesh = SD->GetMesh("ui_board_blank");
		uiComponents_list[COMPONENT_TEXT].mesh = NULL;
		uiComponents_list[COMPONENT_TEXT].textSize = scale.y * 0.5f;
		uiComponents_list[COMPONENT_TEXT].pos.Set(0.3f, 0.5f);
		uiComponents_list[COMPONENT_TEXT].anchorPoint.Set(0, 0);
		if (buttonType == BUTTON_BUILD_LOGS)
			uiComponents_list[COMPONENT_TEXT].text = "Logs";
		else if(buttonType == BUTTON_BUILD_GRANARY)
			uiComponents_list[COMPONENT_TEXT].text = "Granary";
		else if (buttonType == BUTTON_BUILD_WOODSHED)
			uiComponents_list[COMPONENT_TEXT].text = "Woodshed";
		else if (buttonType == BUTTON_BUILD_HOUSE)
			uiComponents_list[COMPONENT_TEXT].text = "House";
		
		break;
	case BUTTON_RESEARCH_COST:
		scale.Set(200, 50);
		pos.Set(x,y);
		anchorPoint.Set(0.5f,0.5f);
		uiComponents_list[COMPONENT_BOARD].mesh = SD->GetMesh("ui_board_blank");
		uiComponents_list[COMPONENT_TEXT].mesh = NULL;
		uiComponents_list[COMPONENT_TEXT].text = "asffds";
		uiComponents_list[COMPONENT_TEXT].textSize = scale.y * 0.5f;
		uiComponents_list[COMPONENT_TEXT].pos.Set(0.2f, 0.5f);
		uiComponents_list[COMPONENT_TEXT].anchorPoint.Set(0, 0);
		break;
	}
	this->buttonType = buttonType;
	
	float ratio = scale.x / scale.y;
}

UIGameButton::~UIGameButton()
{
}

void UIGameButton::Update(float dt)
{
	UIBase::UpdateTween(dt);
	PostOffice* PO = PostOffice::GetInstance();
	if (this->currentTween == NULL && this->IsMousePressed())
	{
		switch (this->buttonType)
		{
		case BUTTON_DAILYREQUIREMENT:
			PO->Send("Scene"
				, new MessageDisplayDailyRequirement(this));
			break;
		case BUTTON_SELECTED_ALTAR_OFFER:
			PO->Send("Scene"
				, new MessageAltarOffer(MessageAltarOffer::OFFER_FOOD));
			break;
		case BUTTON_SELECTED_RLAB:
			PO->Send("Scene"
				, new MessageResearch());
			break;
		case BUTTON_SELECTED_GENERAL_MOVE:
			PO->Send("Scene"
				, new MessageMoveButton());
			break;
		case BUTTON_SELECTED_CHIEFHUT_BUILD:
			PO->Send("Scene",
				new MessageCreateBuildUIs());
			break;
		case BUTTON_BUILD_LOGS:
			PO->Send("Scene"
				, new MessageBuildBuildings(GameObject::GO_LOGS));
			break;
		case BUTTON_BUILD_GRANARY:
			PO->Send("Scene"
				, new MessageBuildBuildings(GameObject::GO_GRANARY));
			break;
		case BUTTON_BUILD_HOUSE:
			PO->Send("Scene"
				, new MessageBuildBuildings(GameObject::GO_HOUSE));
			break;
		case BUTTON_BUILD_WOODSHED:
			PO->Send("Scene"
				, new MessageBuildBuildings(GameObject::GO_WOODSHED));
			break;
		}
	}
}

void UIGameButton::SetText(std::string text)
{
	uiComponents_list[COMPONENT_TEXT].text = text;
}
