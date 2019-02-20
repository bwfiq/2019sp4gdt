#pragma once
#include "EffectBase.h"

class EffectGridWarning : public EffectBase {
public:
	EffectGridWarning(Vector3 pos, Vector3 size, float alpha = 0.5f);
	virtual ~EffectGridWarning();

	float fElapsedTime;
	Vector3 origScale;
	float origAlpha;
	float fDuration;

	virtual void Update(float dt);
protected:
private:
	//virtual void Render() = 0;
};