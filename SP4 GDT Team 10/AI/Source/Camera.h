#ifndef CAMERA_H
#define CAMERA_H

#include "Vector3.h"

class Camera
{
public:
	enum CAMERA_VIEWANGLE {
		VIEW_TOPDOWN,
		VIEW_ANGLE_1,
		VIEW_ANGLE_2,
		
		VIEW_TOTAL
	};
	Vector3 position;
	Vector3 target;
	Vector3 up;

	float fDistance;

	Camera();
	~Camera();
	virtual void Init(const Vector3& pos, const Vector3& target, const Vector3& up);
	virtual void Reset();
	virtual void Update(double dt);

	void SetCamShake(int shakeType, float intensity, float duration);
private:
	enum CAMERA_SHAKE_TYPE {
		SHAKE_NONE,
		SHAKE_EARTHQUAKE,

		SHAKE_TOTAL
	};
	Vector3 position_goal;
	Vector3 target_goal;

	Vector3 position_velocity;
	Vector3 target_velocity;

	Vector3 default_position;
	Vector3 default_target;
	Vector3 default_up;

	float fCameraBorderMovespeed;
	CAMERA_SHAKE_TYPE shakeType;
	float fShakeIntensity;
	float fShakeDuration;
	CAMERA_VIEWANGLE viewAngle;

	Vector3 position_offset;//used for camera angle

	void SetCameraAngle(CAMERA_VIEWANGLE viewAngle);

	void CalculateUp();
};

#endif