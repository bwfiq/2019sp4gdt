#pragma once
#include "EffectBase.h"

struct GameObject;

class EffectReticle : public EffectBase {
public:
	EffectReticle();
	virtual ~EffectReticle();

	GameObject* selected;
	float reticleThickness;
	float fElapsedTime;

	virtual void Update(float dt);
protected:
private:
	//virtual void Render() = 0;
};