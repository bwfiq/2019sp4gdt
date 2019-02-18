#include "CalamityBase.h"
#include "Mesh.h"

CalamityBase::CalamityBase() :
	bActive(true),
	fElapsedTime(0)
{
}

CalamityBase::~CalamityBase()
{
}

bool CalamityBase::IsDone()
{
	return this->fElapsedTime > this->fCalamityDuration;
}
