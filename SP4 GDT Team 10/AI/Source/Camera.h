#ifndef CAMERA_H
#define CAMERA_H

#include "Vector3.h"

class Camera
{
public:
	Vector3 position;
	Vector3 target;
	Vector3 up;

	float fDistance;

	Camera();
	~Camera();
	virtual void Init(const Vector3& pos, const Vector3& target, const Vector3& up);
	virtual void Reset();
	virtual void Update(double dt);
private:
	Vector3 position_goal;
	Vector3 target_goal;

	Vector3 position_velocity;
	Vector3 target_velocity;

	Vector3 default_position;
	Vector3 default_target;
	Vector3 default_up;

	float fCameraBorderMovespeed;
};

#endif