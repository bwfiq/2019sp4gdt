#pragma once
#include "Vector3.h"
#include <vector>
#include <string>
class Mesh;
class UIBase;

struct UIComponent {//yeaaaaaaaaaahhh this is garbo but wutevz
public:
	enum UI_COMPONENT_TYPE {
		UI_FRAME,//or ui_image wutever u like lulz
		UI_TEXT,
		UI_BUTTON,

		UI_TOTAL
	};
	UI_COMPONENT_TYPE type;
	Mesh* mesh;
	bool bActive;
	Vector3 pos;
	Vector3 scale;
	float fRotation;
	Vector3 anchorPoint;
	std::string text;

	UIComponent();
	~UIComponent();

	//bool IsMouseHovered(UIBase* uiBase);
};