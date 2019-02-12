#pragma once
#include "SingletonTemplate.h"
#include "Vector3.h"
#include "Camera.h"
struct MousePicker : public Singleton<MousePicker>
{
	friend Singleton<MousePicker>; 
public:
	
private:
	MousePicker();
	~MousePicker();
	Vector3 mouseRay;
	Camera* currentCam;
};