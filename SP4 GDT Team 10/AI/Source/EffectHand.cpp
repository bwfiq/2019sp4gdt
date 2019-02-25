#include "EffectHand.h"
#include "Mesh.h"
#include "SceneData.h"
#include "MouseController.h"
#include "MousePicker.h"

#include "EffectManager.h"
#include "EffectCloud.h"

EffectHand::EffectHand(Camera* cam) :
	EffectBase()
	, defaultPosOffset(0, 0.25f, 0)
	, defaultScale(0.65f, 0.65f, 0.65f)
	, defaultRot()
	, grabPosOffset(0.15f, 2, 0)
	, grabScale(1, 1, 1)
	, grabRot(0, 0, -90)
{
	SceneData* SD = SceneData::GetInstance();
	mesh = SD->GetMesh("hand_point");
	bLightEnabled = true;
	this->cameraObj = cam;
	this->fAlpha = 1;
	this->state = HAND_DEFAULT;
	posOffset = defaultPosOffset;
}

EffectHand::~EffectHand()
{
}

void EffectHand::Update(float dt)
{
	SceneData* SD = SceneData::GetInstance();
	MouseController* MC = MouseController::GetInstance();
	float alpha = Math::Min(1.f, dt * 20.f);
	switch (this->state)
	{
	case HAND_DEFAULT:
		if (MC->IsButtonPressed(MouseController::RMB))
		{
			mesh = SD->GetMesh("hand_grab");
			for (int i = 0; i < 5; ++i)
			{
				EffectManager::GetInstance()->AddEffect(new EffectCloud(
					this->pos, Math::RandFloatMinMax(0.4, 0.8f)
				));
			}
		}
		else if (MC->IsButtonReleased(MouseController::RMB))
		{
			mesh = SD->GetMesh("hand_point");
		}
		if (rotation != defaultRot)
		{
			rotation.lerp(defaultRot, alpha*0.75f);
		}
		if (scale != defaultScale)
		{
			scale.lerp(defaultScale, alpha);
		}
		//if (posOffset != defaultPosOffset)
		{
			posOffset.lerp(defaultPosOffset + Vector3(0, 0.1f * cosf(SD->GetElapsedTime()), 0) , alpha);
		}
		break;
	case HAND_GRAB_OBJECT:
		mesh = SD->GetMesh("hand_grab");
		if (rotation != grabRot)
		{
			rotation.lerp(grabRot, alpha*0.75f);
		}
		if (scale != grabScale)
		{
			scale.lerp(grabScale, alpha);
		}
		if (posOffset != grabPosOffset)
		{
			posOffset.lerp(grabPosOffset, alpha);
		}

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
	//pos = SD->GetMousePos_World() + posOffset + dir * (-MC->GetMouseScrollStatus(MouseController::SCROLL_TYPE_YOFFSET) + 4) * 0.7f;
	pos = SD->GetMousePos_World() + posOffset;

}

void EffectHand::SetState(HAND_STATE state)
{
	this->state = state;
	if (state == HAND_DEFAULT && !MouseController::GetInstance()->IsButtonDown(MouseController::RMB))
	{
		mesh = SceneData::GetInstance()->GetMesh("hand_point");
	}
}
