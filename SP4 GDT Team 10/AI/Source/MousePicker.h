#pragma once
#include "SingletonTemplate.h"
#include "Vector3.h"
#include "Camera.h"
#include "MatrixStack.h"
class MousePicker : public Singleton<MousePicker>
{
	friend Singleton<MousePicker>;
public:
	void Init();
	void Update(float dt);


	void SetWindowWidth(float wWidth) { this->fWindowWidth = wWidth; }
	void SetWindowHeight(float wHeight) { this->fWindowHeight = wHeight; }
	void SetProjectionStack(MS &pS) { this->projectionStack = &pS; }
	void SetViewStack(MS &vS) { this->viewStack = &vS; }

	Vector3 GetMouseRay() const { return this->mouseRay; }
private:
	MousePicker();
	~MousePicker();
	Vector3 mouseRay;
	MS* projectionStack;
	MS* viewStack;
	float fWindowWidth;
	float fWindowHeight;
};