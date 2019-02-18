#pragma once
#include "Vector3.h"
#include <vector>
#include <string>
//#include "UIComponent.h"
class Mesh;

struct UIComponent {//yeaaaaaaaaaahhh this is garbo but wutevz
public:
	Mesh* mesh;
	bool bActive;
	Vector3 pos;
	Vector3 scale;
	float fRotation;
	Vector3 anchorPoint;

	float textSize;
	std::string text;

	float alpha;

	UIComponent() : fRotation(0), bActive(true), mesh(NULL), scale(1, 1, 1), alpha(1.f) {};
	~UIComponent() {};
};

class UIBase {
public:
	UIBase();
	virtual ~UIBase();

	std::string uiName;

	std::vector<UIComponent> uiComponents_list;

	Vector3 pos;
	Vector3 scale;
	Vector3 anchorPoint;
	float fRotation;
	bool bActive;
	bool bLightEnabled;
	bool bIsDone;

	virtual void Update(float dt) = 0;

	//!!!!!! THIS THING ONLY CALCULATES THE MOUSE IN THE UIBASE, NOT THE INDIVIDUAL COMPONENT !!!!!!
	bool IsMouseHovered();
	bool IsMousePressed();
protected:
private:
	//virtual void Render() = 0;
};