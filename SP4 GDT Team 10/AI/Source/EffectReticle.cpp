#include "EffectReticle.h"
#include "Mesh.h"
#include "SceneData.h"
#include "MouseController.h"
#include "MousePicker.h"
#include "GameObject.h"

EffectReticle::EffectReticle(RETICLE_TYPE type) :
	EffectBase(),
	type(type)
{
	SceneData* SD = SceneData::GetInstance();
	rotation.Set(90, 0, 0);
	reticleThickness = 3;
	fElapsedTime = 0;
	selected = NULL;
	if (type == RETICLE_OUTER)
	{
		mesh = SD->GetMesh("reticle");
	}
	else if (type == RETICLE_INNER)
	{
		mesh = SD->GetMesh("reticle_cross");
		rotation.z = 45;
		scale.Set(0.5f, 0.5f, 0.5f);
		reticleThickness = 5;
	}
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
	if (type == RETICLE_OUTER)
	{
		Vector3 goal;
		if (selected == NULL)
		{
			goal = SD->GetMousePos_World() + Vector3(0, 0.01f, 0);
			if (pos != goal)
			{
				pos.lerp(goal, Math::Min(1.f, dt*10.f));
			}
			scale = Vector3(1, 1, 1) * (1 + 0.2f * cosf(fElapsedTime * 5));
			rotation.z = 0;
		}
		else
		{
			goal = selected->pos;
			if (pos != goal)
			{
				pos.lerp(goal, Math::Min(1.f, dt*12.f));
			}
			scale = Vector3(1.5f, 1.5f, 1.5f) * (1 + 0.2f * cosf(fElapsedTime * 5));
			rotation.z -= dt * 125;
		}
	}
	else if (type == RETICLE_INNER)
	{
		pos = SD->GetMousePos_World() + Vector3(0, 0.01f, 0);
		scale = Vector3(0.4f, 0.4f, 0.4f) * (1 + 0.1f * cosf(-Math::HALF_PI + fElapsedTime * 5));
	}
}
