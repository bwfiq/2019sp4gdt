#pragma once
#include "EffectBase.h"

class EffectMovable : public EffectBase {
public:
	EffectMovable();
	virtual ~EffectMovable();

	Vector3 vel;
	Vector3 acc;

	virtual void Update(float dt);
protected:
private:
	//virtual void Render() = 0;
};