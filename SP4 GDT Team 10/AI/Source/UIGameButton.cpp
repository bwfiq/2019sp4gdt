#include "UIGameButton.h"
#include "SceneData.h"
#include "PostOffice.h"
#include "ConcreteMessages.h"

UIGameButton::UIGameButton(BUTTON_TYPE buttonType) :
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
		pos.Set(1, 0.25f);
		scale.Set(250, 100);
		anchorPoint.Set(1, 1);
		uiComponents_list[COMPONENT_BOARD].mesh = SD->GetMesh("ui_board_blank");
		uiComponents_list[COMPONENT_TEXT].mesh = NULL;
		uiComponents_list[COMPONENT_TEXT].text = "Offer Food";
		uiComponents_list[COMPONENT_TEXT].textSize = scale.y * 0.35f;
		uiComponents_list[COMPONENT_TEXT].pos.Set(0.1f, 0.5f);
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
	PostOffice* PO = PostOffice::GetInstance();
	if (this->IsMousePressed())
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
		}
	}
}

void UIGameButton::SetText(std::string text)
{
	uiComponents_list[COMPONENT_TEXT].text = text;
}
