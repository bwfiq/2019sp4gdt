#pragma once
#include "EffectBase.h"

class Camera;

class EffectHand : public EffectBase {
public:
	enum HAND_STATE {
		HAND_DEFAULT,
		HAND_GRAB_OBJECT,
	};
	EffectHand(Camera* camera);
	virtual ~EffectHand();

	Camera* cameraObj;

	virtual void Update(float dt);
	void SetState(HAND_STATE state);
protected:
	HAND_STATE state;
private:
	//virtual void Render() = 0;
};