#pragma once
#include "UIBase.h"

struct GameObject;

class UIGameButton : public UIBase
{
public:
	enum BUTTON_TYPE {
		BUTTON_DAILYREQUIREMENT,
		BUTTON_SELECTED_GENERAL_MOVE,
		BUTTON_SELECTED_ALTAR_OFFER,
		BUTTON_SELECTED_CHIEFHUT_BUILD,
		BUTTON_SELECTED_RLAB,

		BUTTON_RESEARCH_COST,

		BUTTON_BUILD_LOGS,
		BUTTON_BUILD_GRANARY,
		BUTTON_BUILD_WOODSHED,
		BUTTON_BUILD_HOUSE,

		BUTTON_TOTAL
	};
	UIGameButton(BUTTON_TYPE buttonType, float x = 0, float y=0, unsigned order = 0, GameObject* go = NULL);
	~UIGameButton();

	virtual void Update(float dt);
	void SetText(std::string text);

	//Order of ui components matter
	BUTTON_TYPE buttonType;
	enum UI_GAMEBUTTON_COMPONENTS {
		COMPONENT_BOARD,
		COMPONENT_TEXT,

		COMPONENT_TOTAL,
	};
private:
};