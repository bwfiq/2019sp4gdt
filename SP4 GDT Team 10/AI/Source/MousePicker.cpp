#include "MousePicker.h"
#include "MouseController.h"
#include "Application.h"


MousePicker::MousePicker() :
	projectionStack(NULL),
	viewStack(NULL),
	mouseRay(0,0,-1),
	fWindowWidth(0),
	fWindowHeight(0)
{

}

MousePicker::~MousePicker()
{
}

void MousePicker::Init()
{
	fWindowWidth = Application::GetInstance().GetWindowWidth();
	fWindowHeight = Application::GetInstance().GetWindowHeight();

}
void MousePicker::Update(float dt)
{
	fWindowWidth = Application::GetInstance().GetWindowWidth();
	fWindowHeight = Application::GetInstance().GetWindowHeight();
	if (projectionStack != NULL && viewStack != NULL) {
		float mX, mY;
		MouseController::GetInstance()->GetMousePosition(mX, mY);
		mX = (0.5f - mX / fWindowWidth) * 2;
		mY = (0.5f - mY / fWindowHeight) * 2;
		Vector3 ray(-mX, mY, -1);
		ray = projectionStack->Top().GetInverse() * ray;
		ray.Set(ray.x, ray.y, -1);
		ray = viewStack->Top().GetInverse() * ray;
		this->mouseRay = ray.Normalized();
	}
}

Vector3 MousePicker::GetIntersectionWithPlane(Vector3 rayPos, Vector3 planePos, Vector3 planeNormal)
{
	float d = -(planePos.Dot(planeNormal));
	float t = -(rayPos.Dot(planeNormal) + d) / (mouseRay.Dot(planeNormal));
	return rayPos + t * mouseRay;
}