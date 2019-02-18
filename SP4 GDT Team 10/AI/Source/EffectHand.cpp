#include "EffectHand.h"
#include "Mesh.h"
#include "SceneData.h"
#include "MouseController.h"

EffectHand::EffectHand() :
	EffectBase()
{
	SceneData* SD = SceneData::GetInstance();
	mesh = SD->GetMesh("hand_default");
	bLightEnabled = true;
}

EffectHand::~EffectHand()
{
}

void EffectHand::Update(float dt)
{
	SceneData* SD = SceneData::GetInstance();
	MouseController* MC = MouseController::GetInstance();
	if (MC->IsButtonPressed(MouseController::RMB))
	{
		mesh = SD->GetMesh("hand_grab");
	}
	else if (MC->IsButtonReleased(MouseController::RMB))
	{
		mesh = SD->GetMesh("hand_default");
	}
	pos = SD->GetMousePos_World() + Vector3(0, 0.5f, 0);

}
