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
	Vector3 posOffset;

	virtual void Update(float dt);
	void SetState(HAND_STATE state);
protected:
	HAND_STATE state;
private:
	//virtual void Render() = 0;
	const Vector3 defaultPosOffset;
	const Vector3 defaultScale;
	const Vector3 defaultRot;

	const Vector3 grabPosOffset;
	const Vector3 grabScale;
	const Vector3 grabRot;
};