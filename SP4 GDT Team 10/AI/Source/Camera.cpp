#include "Camera.h"
#include "Application.h"
#include "Mtx44.h"
#include "MouseController.h"
#include "KeyboardController.h"
#include "Mtx44.h"
#include "SceneData.h"

Camera::Camera()
{
	Reset();
}

Camera::~Camera()
{
}

void Camera::Init(const Vector3& pos, const Vector3& target, const Vector3& up)
{
	this->position = this->position_goal = this->default_position = pos;
	this->target = this->target_goal = this->default_target = target;
	this->up = this->default_up = up;
	fCameraBorderMovespeed = 4.f;
	SetCameraAngle(VIEW_ANGLE_1);
	bounds.Set(200, 0, 200);
}

void Camera::Reset()
{
	position_goal = default_position;
	target_goal = default_target;
	position_velocity.SetZero();
	target_velocity.SetZero();
	up = default_up;
	fDistance = 1;
}

void Camera::Update(double dt)
{
	MouseController* MC = MouseController::GetInstance();
	KeyboardController* KC = KeyboardController::GetInstance();
	Vector3 mousePos,mouseDelta;
	MC->GetMousePosition(mousePos.x, mousePos.y);
	MC->GetMouseDelta(mouseDelta.x, mouseDelta.z);
	int windowWidth = Application::GetInstance().GetWindowWidth();
	int windowHeight = Application::GetInstance().GetWindowHeight();
	float mouseScroll = (-MC->GetMouseScrollStatus(MouseController::SCROLL_TYPE_YOFFSET) + 4) * 0.5f;
	//Input stuff
	if (MC->IsButtonUp(MouseController::RMB) &&
		mousePos.x > 0 && mousePos.x < windowWidth &&
		mousePos.y > 0 && mousePos.y < windowHeight)
	{
		float diff;//this gon be used to calculate the difference, basically done to avoid using Math::Min twice
		if (mousePos.x > windowWidth * 0.95f)
		{
			diff = position_velocity.x;
			position_velocity.x = Math::Min(position_velocity.x + 20 * (float)dt, fCameraBorderMovespeed * mouseScroll);
			diff = position_velocity.x - diff;
			target_velocity.x += diff;
		}
		else if (mousePos.x < windowWidth * 0.05f)
		{
			diff = position_velocity.x;
			position_velocity.x = Math::Max(position_velocity.x - 20 * (float)dt, -fCameraBorderMovespeed * mouseScroll);
			diff = position_velocity.x - diff;
			target_velocity.x += diff;
		}
		if (mousePos.y > windowHeight * 0.95f)
		{
			diff = position_velocity.z;
			position_velocity.z = Math::Min(position_velocity.z + 20 * (float)dt, fCameraBorderMovespeed * mouseScroll);
			diff = position_velocity.z - diff;
			target_velocity.z += diff;
		}
		else if (mousePos.y < windowHeight * 0.05f)
		{
			diff = position_velocity.z;
			position_velocity.z = Math::Max(position_velocity.z - 20 * (float)dt, -fCameraBorderMovespeed * mouseScroll);
			diff = position_velocity.z - diff;
			target_velocity.z += diff;
		}
	}
	if (MC->IsButtonPressed(MouseController::RMB))
	{
		position_velocity.SetZero();
		target_velocity.SetZero();
	}
	if (!mouseDelta.IsZero())
	{
		mouseDelta *= -0.005f * mouseScroll;
		if (MC->IsButtonDown(MouseController::RMB))
		{
			position_goal += mouseDelta;
			target_goal += mouseDelta;
		}
		else if (MC->IsButtonReleased(MouseController::RMB))
		{
			float speed = Math::Clamp(mouseDelta.Length()*20, 40.f , 75.f);
			position_velocity = mouseDelta * speed;
			target_velocity = mouseDelta * speed;
		}
	}
	Vector3 keyboardMovementDelta;
	if (KC->IsKeyPressed('W'))
	{
		float movement = -(0.25f + 0.5f * mouseScroll);
		keyboardMovementDelta.z += movement;
		keyboardMovementDelta.z += movement;
	}
	else if (KC->IsKeyPressed('S'))
	{
		float movement = (0.25f + 0.5f * mouseScroll);
		keyboardMovementDelta.z += movement;
		keyboardMovementDelta.z += movement;
	}
	if (KC->IsKeyPressed('A'))
	{
		float movement = -(0.25f + 0.5f * mouseScroll);
		keyboardMovementDelta.x += movement;
		keyboardMovementDelta.x += movement;
	}
	else if (KC->IsKeyPressed('D'))
	{
		float movement = (0.25f + 0.5f * mouseScroll);
		keyboardMovementDelta.x += movement;
		keyboardMovementDelta.x += movement;
	}
	if (KC->IsKeyPressed(VK_TAB))
	{
		this->SetCameraAngle((CAMERA_VIEWANGLE)(((int)viewAngle + 1) % (int)VIEW_TOTAL));
	}

	switch (this->shakeType) 
	{
	case SHAKE_EARTHQUAKE:
	{
		Vector3 randShake(Math::RandFloatMinMax(-fShakeIntensity, fShakeIntensity), 0, Math::RandFloatMinMax(-fShakeIntensity, fShakeIntensity));
		position += randShake;
		target += randShake * 0.5f;
	}
		
		break;
	case SHAKE_NONE:
		break;
	}
	
	//Updat stuff
	if (!keyboardMovementDelta.IsZero())//if der was keyboard input thin go
	{
		position_goal += keyboardMovementDelta;
		target_goal += keyboardMovementDelta;
	}
	if (!position_velocity.IsZero())
	{
		//std::cout << "asd" << std::endl;
		position_goal += position_velocity * dt;
		Vector3 origVel(position_velocity.Normalized());
		position_velocity += -position_velocity.Normalized() * 5 * dt;
		if (position_velocity.IsZero() || origVel.Dot(position_velocity.Normalized()) < -0.9f)
			position_velocity.SetZero();
	}
	if (!target_velocity.IsZero())
	{
		target_goal += target_velocity * dt;
		Vector3 origVel(target_velocity.Normalized());
		target_velocity += -target_velocity.Normalized() * 5 * dt;
		if (target_velocity.IsZero() || origVel.Dot(target_velocity.Normalized()) < -0.9f)
			target_velocity.SetZero();
	}

	//constrains/boundaries
	Vector3 boundaryCrossedDistance;//how much was the boundaries crossed
	//if (position_goal.x < -bounds.x)
	//	position_goal.x = -bounds.x;
	//else if(position_goal.x > bounds.x)
	//	position_goal.x = bounds.x;
	//if (position_goal.z < -bounds.z)
	//	position_goal.z = -bounds.z;
	//else if (position_goal.z > bounds.z)
	//	position_goal.z = bounds.z;
	if (position_goal.x < -bounds.x)
		boundaryCrossedDistance.x += (position_goal.x + bounds.x);
	else if(position_goal.x > bounds.x)
		boundaryCrossedDistance.x += (position_goal.x - bounds.x);
	if (position_goal.z < -bounds.z)
		boundaryCrossedDistance.z += (position_goal.z + bounds.z);
	else if (position_goal.z > bounds.z)
		boundaryCrossedDistance.z += (position_goal.z - bounds.z);
	if (!boundaryCrossedDistance.IsZero())
	{
		position_goal -= boundaryCrossedDistance;
		target_goal -= boundaryCrossedDistance;
	}


	Vector3 position_goal_with_zoom = position_goal + Vector3(0, position_goal.y, 0) * mouseScroll + this->position_offset;
	if (position != position_goal_with_zoom)
	{
		position.lerp(position_goal_with_zoom, Math::Min((float)dt*20.f, 1.f));
	}
	if (target != target_goal)
	{
		target.lerp(target_goal, Math::Min((float)dt*15.5f, 1.f));
	}
	Vector3 view = (target_goal - position_goal_with_zoom).Normalized();
	Vector3 right = view.Cross(up);
	right.y = 0;
	right.Normalize();
	up = right.Cross(view).Normalized();
	view = view;
	target_goal = position_goal + view;

	SceneData::GetInstance()->SetCamPosition(this->position);
	SceneData::GetInstance()->SetCamTarget(this->target);
}

void Camera::SetCamShake(int shakeType, float intensity, float duration)
{
	if (intensity <= 0)
	{
		this->shakeType = SHAKE_NONE;
		return;
	}
	this->shakeType = (CAMERA_SHAKE_TYPE)shakeType;
	this->fShakeIntensity = intensity;
	this->fShakeDuration = duration;
}

void Camera::SetCamBounds(Vector3 bounds)
{
	this->bounds = bounds;
}

void Camera::SetCameraAngle(CAMERA_VIEWANGLE viewAngle)
{
	this->viewAngle = viewAngle;
	switch (this->viewAngle)
	{
	case VIEW_TOPDOWN:
		this->position_offset = Vector3(0, 2, 0);
		break;
	case VIEW_ANGLE_1:
		this->position_offset = Vector3(0, 1, 3);
		break;
	case VIEW_ANGLE_2:
		this->position_offset = Vector3(0, -2, 5);
		break;
	}
}

void Camera::CalculateUp()
{
	Vector3 lookVector = (target_goal - position_goal).Normalized();
	Vector3 right = lookVector.Cross(up);
	right.y = 0;
	right.Normalize();
	up = right.Cross(lookVector).Normalized();
}
