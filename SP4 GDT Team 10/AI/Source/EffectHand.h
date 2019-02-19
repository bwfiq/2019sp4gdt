#pragma once
#include "EffectBase.h"

class Camera;

class EffectHand : public EffectBase {
public:
	EffectHand(Camera* camera);
	virtual ~EffectHand();

	Camera* cameraObj;

	virtual void Update(float dt);
protected:
private:
	//virtual void Render() = 0;
};