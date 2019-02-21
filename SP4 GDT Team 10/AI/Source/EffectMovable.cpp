#include "EffectMovable.h"

EffectMovable::EffectMovable() :
	EffectBase()
{
}

EffectMovable::~EffectMovable()
{
}

void EffectMovable::Update(float dt)
{
	vel += acc * dt;
	pos += vel * dt;
}
