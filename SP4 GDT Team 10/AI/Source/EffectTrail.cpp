#include "EffectTrail.h"
#include "SceneData.h"
#include "MousePicker.h"

EffectTrail::EffectTrail(EffectBase* toTrace)
{
	mesh = SceneData::GetInstance()->GetMesh("whiteray");
	fLineThickness = 10;
	this->toTrace = toTrace;
	bLightEnabled = false;
	SetTrailLifetime(0.175f);
	//Vector3 mousePos = MousePicker::GetInstance()->GetIntersectionWithPlane(cameraObj->position, Vector3(0, 0, 0), Vector3(0, 1, 0));
}

EffectTrail::~EffectTrail()
{
}

void EffectTrail::Update(float dt)
{
	//current locked to mouse position but wutever
	//Vector3 mousePos = SceneData::GetInstance()->GetMousePos_World();
	trailPoints.pop_front();
	//trailPoints.push_back(mousePos + Vector3(0,0.15f,0));
	trailPoints.push_back(toTrace->pos);
}

void EffectTrail::SetTrailLifetime(float lifeTime)
{
	fLifetime = lifeTime;
	iTotalPoints = floorf(60.f * fLifetime);
	trailPoints.clear();
	if (this->toTrace)
	{
		for (int i = 0; i < iTotalPoints; ++i)
		{
			trailPoints.push_back(toTrace->pos);
		}
	}
	else
	{
		for (int i = 0; i < iTotalPoints; ++i)
		{
			trailPoints.push_back(Vector3());
		}
	}
	
}
