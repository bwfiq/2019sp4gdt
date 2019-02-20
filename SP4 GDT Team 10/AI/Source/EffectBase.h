#pragma once
#include "Vector3.h"
class Mesh;

class EffectBase {
public:
	EffectBase();
	virtual ~EffectBase();

	Vector3 pos;
	Vector3 scale;
	Vector3 rotation;
	bool bActive;
	bool bLightEnabled;
	bool bIsDone;
	Mesh* mesh;
	float fAlpha;

	virtual void Update(float dt) = 0;
protected:
private:
	//virtual void Render() = 0;
};