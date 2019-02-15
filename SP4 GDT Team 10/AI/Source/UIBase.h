#pragma once
#include "Vector3.h"
#include <vector>
#include <string>
#include "UIComponent.h"
class Mesh;

class UIBase {
public:
	UIBase();
	virtual ~UIBase();

	std::vector<UIComponent> uiComponents_list;

	Vector3 pos;
	Vector3 scale;
	Vector3 anchorPoint;
	float fRotation;
	bool bActive;
	bool bLightEnabled;

	virtual void Update(float dt) = 0;

	//!!!!!! THIS THING ONLY CALCULATES THE MOUSE IN THE UIBASE, NOT THE INDIVIDUAL COMPONENT !!!!!!
	bool IsMouseHovered();
	bool IsMousePressed();
protected:
private:
	//virtual void Render() = 0;
};