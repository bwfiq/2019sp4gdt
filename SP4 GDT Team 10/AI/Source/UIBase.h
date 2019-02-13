#pragma once
#include "Vector3.h"
#include <vector>
class Mesh;

struct UIComponent {//yeaaaaaaaaaahhh this is garbo but wutevz
public:
	Mesh* mesh;
	bool bActive;
	Vector3 pos;
	Vector3 scale;
	float fRotation;
	Vector3 anchorPoint;

	UIComponent() : fRotation(0), bActive(true), mesh(NULL), scale(1, 1, 1) {};
	~UIComponent() {};
};

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
protected:
private:
	//virtual void Render() = 0;
	virtual void Update(float dt) = 0;
};