#pragma once
#include "EffectBase.h"

struct GameObject;

class EffectReticle : public EffectBase {
public:
	enum RETICLE_TYPE {
		RETICLE_OUTER,
		RETICLE_INNER
	};
	EffectReticle(RETICLE_TYPE type = RETICLE_OUTER);
	virtual ~EffectReticle();

	GameObject* selected;
	float reticleThickness;
	float fElapsedTime;

	virtual void Update(float dt);
protected:
	RETICLE_TYPE type;
private:
	//virtual void Render() = 0;
};