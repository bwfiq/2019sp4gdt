#pragma once
#include "UIBase.h"

struct GameObject;

class UIGameText : public UIBase
{
public:
	enum TEXT_TYPE {
		TEXT_DAILYREQUIREMENT,
		TEXT_SELECTED_ALTAR,
		TEXT_SELECTED_RLAB,
		TEXT_SELECTED_BUILDING,
		TEXT_SELECTED_VILLAGER,
		TEXT_SELECTED_ENVIRONMENT,

		TEXT_TOTAL
	};
	UIGameText(TEXT_TYPE textType, GameObject* go = NULL);
	~UIGameText();

	virtual void Update(float dt);
	void SetText(std::string text);

	//Order of ui components matter
	TEXT_TYPE textType;
	GameObject* go;
	enum UI_GAMETEXT_COMPONENTS {
		COMPONENT_BOARD,
		COMPONENT_TEXT_1,
		COMPONENT_TEXT_2,
		COMPONENT_TEXT_3,
		COMPONENT_TEXT_4,
		COMPONENT_TEXT_5,


		COMPONENT_TOTAL,
	};
private:
};