#include "UIBase.h"
#include "Mesh.h"

UIBase::UIBase() :
	bLightEnabled(false),
	bActive(true),
	fRotation(0),
	scale(1, 1, 1)
{
}

UIBase::~UIBase()
{
}
