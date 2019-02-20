#include "EffectReticle.h"
#include "Mesh.h"
#include "SceneData.h"
#include "MouseController.h"
#include "MousePicker.h"
#include "GameObject.h"

EffectReticle::EffectReticle() :
	EffectBase()
{
	SceneData* SD = SceneData::GetInstance();
	mesh = SD->GetMesh("reticle");
	rotation.Set(90, 0, 0);
	reticleThickness = 3;
	fElapsedTime = 0;
	selected = NULL;
	//scale.Set(0.5f, 0.5f, 0.5f);
}
EffectReticle::~EffectReticle()
{
}

void EffectReticle::Update(float dt)
{
	fElapsedTime += dt;
	SceneData* SD = SceneData::GetInstance();
	MouseController* MC = MouseController::GetInstance();
	if (selected == NULL)
	{
		pos = SD->GetMousePos_World() + Vector3(0, 0.01f, 0);
		scale = Vector3(1, 1, 1) * (1 + 0.2f * cosf(fElapsedTime * 5));
		rotation.z = 0;
	}
	else
	{
		pos = selected->pos;
		scale = Vector3(1.5f, 1.5f, 1.5f) * (1 + 0.2f * cosf(fElapsedTime * 5));
		rotation.z -= dt * 125;
	}

}
