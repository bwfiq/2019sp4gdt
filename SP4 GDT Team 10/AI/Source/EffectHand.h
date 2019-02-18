#pragma once
#include "EffectBase.h"

class EffectHand : public EffectBase {
public:
	EffectHand();
	virtual ~EffectHand();

	virtual void Update(float dt);
protected:
private:
	//virtual void Render() = 0;
};