#include "EffectBase.h"
#include "Mesh.h"

EffectBase::EffectBase() :
	bLightEnabled(false),
	bActive(true),
	scale(1, 1, 1)
{
}

EffectBase::~EffectBase()
{
}
