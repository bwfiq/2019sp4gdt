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
	this->state = HAND_DEFAULT;
}

EffectHand::~EffectHand()
{
}

void EffectHand::Update(float dt)
{
	SceneData* SD = SceneData::GetInstance();
	MouseController* MC = MouseController::GetInstance();
	Vector3 posOffset;
	switch (this->state)
	{
	case HAND_DEFAULT:
		if (MC->IsButtonPressed(MouseController::RMB))
		{
			mesh = SD->GetMesh("hand_grab");
		}
		else if (MC->IsButtonReleased(MouseController::RMB))
		{
			mesh = SD->GetMesh("hand_default");
		}
		if (!rotation.IsZero())
		{
			rotation.lerp(Vector3(), Math::Min(1.f, dt * 20.f));
		}
		break;
	case HAND_GRAB_OBJECT:
		mesh = SD->GetMesh("hand_grab");
		if (rotation != Vector3(0, 0, -90))
		{
			rotation.lerp(Vector3(0, 0, -90), Math::Min(1.f, dt * 20.f));
		}
		posOffset.Set(0.5f, 0, -0.5f);
		break;
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
	pos = SD->GetMousePos_World() + posOffset + dir * (-MC->GetMouseScrollStatus(MouseController::SCROLL_TYPE_YOFFSET) + 4) * 0.7f;
	pos.z += 0.2f;

}

void EffectHand::SetState(HAND_STATE state)
{
	this->state = state;
	if (state == HAND_DEFAULT && !MouseController::GetInstance()->IsButtonDown(MouseController::RMB))
	{
		mesh = SceneData::GetInstance()->GetMesh("hand_default");
	}
}
