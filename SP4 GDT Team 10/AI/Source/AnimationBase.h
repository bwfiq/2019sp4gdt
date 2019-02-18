#pragma once
#include "Mtx44.h"
#include "Vector3.h"

class AnimationBase {
public:
	AnimationBase();
	virtual ~AnimationBase();

	bool active;

	Mtx44 Translate;
	Mtx44 TranslateOffset;
	Mtx44 RevertTranslateOffset;
	Mtx44 Rotate;
	Mtx44 Scale;
	Mtx44 CurrentTransformation;

	virtual Mtx44 GetCurrentTransformation(void);
	virtual void Update(float dt);
protected:
private:
	//virtual void Render() = 0;
};