#include "Camera.h"
#include "Application.h"
#include "Mtx44.h"
#include "MouseController.h"
#include "KeyboardController.h"
#include "Mtx44.h"

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
	MouseController* MS = MouseController::GetInstance();
	KeyboardController* KC = KeyboardController::GetInstance();
	Vector3 mousePos,mouseDelta;
	MS->GetMousePosition(mousePos.x, mousePos.y);
	MS->GetMouseDelta(mouseDelta.x, mouseDelta.z);
	int windowWidth = Application::GetInstance().GetWindowWidth();
	int windowHeight = Application::GetInstance().GetWindowHeight();
	float mouseScroll = (-MS->GetMouseScrollStatus(MouseController::SCROLL_TYPE_YOFFSET) + 4) * 0.5f;
	//Input stuff
	if (MS->IsButtonUp(MouseController::RMB) &&
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
	if (MS->IsButtonPressed(MouseController::RMB))
	{
		position_velocity.SetZero();
		target_velocity.SetZero();
	}
	if (!mouseDelta.IsZero())
	{
		mouseDelta *= -0.005f * mouseScroll;
		if (MS->IsButtonDown(MouseController::RMB))
		{
			position_goal += mouseDelta;
			target_goal += mouseDelta;
		}
		else if (MS->IsButtonReleased(MouseController::RMB))
		{
			float speed = Math::Clamp(mouseDelta.Length()*20, 40.f , 75.f);
			position_velocity = mouseDelta * speed;
			target_velocity = mouseDelta * speed;
		}
	}
	//if (KC->IsKeyPressed('Q'))
	//{
	//	Vector3 view = (target_goal - position_goal).Normalized();
	//	float yaw = 90;
	//	Mtx44 rotation;
	//	rotation.SetToRotation(yaw, 0, 1, 0);
	//	view = rotation * view;
	//	target_goal = position_goal + view;
	//	CalculateUp();
	//}
	//else if (KC->IsKeyPressed('E'))
	//{

	//}

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
	Vector3 position_goal_with_zoom = position_goal + Vector3(0, position_goal.y, 0) * mouseScroll;
	if (position != position_goal_with_zoom)
	{
		position.lerp(position_goal_with_zoom, Math::Min((float)dt*20.f, 1.f));
	}
	if (target != target_goal)
	{
		target.lerp(target_goal, Math::Min((float)dt*15.5f, 1.f));
	}
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

void Camera::CalculateUp()
{
	Vector3 lookVector = (target_goal - position_goal).Normalized();
	Vector3 right = lookVector.Cross(up);
	right.y = 0;
	right.Normalize();
	up = right.Cross(lookVector).Normalized();
}
