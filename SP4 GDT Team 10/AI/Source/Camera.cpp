#include "Camera.h"
#include "Application.h"
#include "Mtx44.h"
#include "MouseController.h"

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
	Vector3 mouseDelta;
	MS->GetMouseDelta(mouseDelta.x, mouseDelta.z);
	//Input stuff
	if (MS->IsButtonPressed(MouseController::RMB))
	{
		position_velocity.SetZero();
		target_velocity.SetZero();
	}
	if (!mouseDelta.IsZero())
	{
		mouseDelta *= -0.005f;
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
	
	
	//Updat stuff
	if (!position_velocity.IsZero())
	{
		//std::cout << "asd" << std::endl;
		position_goal += position_velocity * dt;
		Vector3 origVel(position_velocity.Normalized());
		position_velocity += -position_velocity.Normalized() * 4 * dt;
		std::cout << position_velocity << std::endl;
		if (position_velocity.IsZero() || origVel.Dot(position_velocity.Normalized()) < -0.9f)
			position_velocity.SetZero();
	}
	if (!target_velocity.IsZero())
	{
		target_goal += target_velocity * dt;
		Vector3 origVel(target_velocity.Normalized());
		target_velocity += -target_velocity.Normalized() * 4 * dt;
		if (target_velocity.IsZero() || origVel.Dot(target_velocity.Normalized()) < -0.9f)
			target_velocity.SetZero();
	}
	if (position != position_goal)
	{
		position.lerp(position_goal, Math::Min((float)dt*20.f, 1.f));
	}
	if (target != target_goal)
	{
		target.lerp(target_goal, Math::Min((float)dt*12.5f, 1.f));
	}
}