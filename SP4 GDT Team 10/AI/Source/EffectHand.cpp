#include "EffectHand.h"
#include "Mesh.h"
#include "SceneData.h"
#include "MouseController.h"
#include "MousePicker.h"

EffectHand::EffectHand(Camera* cam) :
	EffectBase()
{
	SceneData* SD = SceneData::GetInstance();
	mesh = SD->GetMesh("hand_default");
	bLightEnabled = true;
	this->cameraObj = cam;
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
	
	Vector3 dir = cameraObj->position - SD->GetMousePos_World();
	try
	{
		dir.Normalize();
	}
	catch (DivideByZero)
	{
		dir.SetZero();
	}
	pos = SD->GetMousePos_World() + dir * (-MC->GetMouseScrollStatus(MouseController::SCROLL_TYPE_YOFFSET) + 4) * 0.7f;
	pos.z += 0.2f;

}
