#include "EffectTrail.h"
#include "SceneData.h"
#include "MousePicker.h"

EffectTrail::EffectTrail(Camera* cam)
{
	mesh = SceneData::GetInstance()->GetMesh("whiteray");
	fLifetime = 0.175f;
	fLineThickness = 10;
	iTotalPoints = 60 * fLifetime;
	cameraObj = cam;
	bLightEnabled = false;
	Vector3 mousePos = MousePicker::GetInstance()->GetIntersectionWithPlane(cameraObj->position, Vector3(0, 0, 0), Vector3(0, 1, 0));
	for (int i = 0; i < iTotalPoints; ++i)
	{
		trailPoints.push_back(Vector3());
	}
}

EffectTrail::~EffectTrail()
{
}

void EffectTrail::Update(float dt)
{
	//current locked to mouse position but wutever
	Vector3 mousePos = SceneData::GetInstance()->GetMousePos_World();
	trailPoints.pop_front();
	trailPoints.push_back(mousePos + Vector3(0,0.15f,0));
}
