#include "MouseController.h"

MouseController::MouseController() :
curr_posX(0.0), curr_posY(0.0),
prev_posX(0.0), prev_posY(0.0),
xoffset(0), yoffset(0),
keepMouseCentered(false)
{
	for (int i = 0; i < NUM_MB; ++i) {
		currBtnStatus[i] = false;
		prevBtnStatus[i] = false;
	}
}

MouseController::~MouseController()
{
}

void MouseController::UpdateMousePosition(double _x, double _y)
{
	curr_posX = _x;
	curr_posY = _y;
}

void MouseController::UpdateMouseButtonPressed(int _slot)
{
	currBtnStatus[_slot] = true;
}

void MouseController::UpdateMouseButtonReleased(int _slot)
{
	currBtnStatus[_slot] = false;
}

void MouseController::UpdateMouseScroll(const double xoffset, const double yoffset)
{
	this->xoffset += xoffset;
	if (this->xoffset > 3.0)
		this->xoffset = 3.0;
	else if (this->xoffset < -3.0)
		this->xoffset = -3.0;
	this->yoffset += yoffset;
	if (this->yoffset > 3.0)
		this->yoffset = 3.0;
	else if (this->yoffset < -3.0)
		this->yoffset = -3.0;
}

void MouseController::EndFrameUpdate()
{
	prev_posX = curr_posX;
	prev_posY = curr_posY;
	for (int i = 0; i < NUM_MB; ++i) {
		prevBtnStatus[i] = currBtnStatus[i];
	}
}

bool MouseController::IsButtonDown(unsigned char _slot)
{
	return currBtnStatus[_slot];
}

bool MouseController::IsButtonUp(unsigned char _slot)
{
	return !(IsButtonDown(_slot));
}

bool MouseController::IsButtonPressed(unsigned char _slot)
{
	// True if currently button is down, previously is up
	return IsButtonDown(_slot) && !prevBtnStatus[_slot];
}

bool MouseController::IsButtonReleased(unsigned char _slot)
{
	// True if currently button is up, previously is down
	return IsButtonUp(_slot) && prevBtnStatus[_slot];
}

double MouseController::GetMouseScrollStatus(SCROLL_TYPE _scrolltype)
{
	if (_scrolltype == SCROLL_TYPE_XOFFSET)
	{
		return xoffset;
	}
	else if (_scrolltype == SCROLL_TYPE_YOFFSET)
	{
		return yoffset;
	}
	else
	{
		return 0.0;
	}
}

void MouseController::GetMousePosition(float& _resultX, float& _resultY)
{
	_resultX = (float)curr_posX;
	_resultY = (float)curr_posY;
}

void MouseController::GetMousePosition(double& _resultX, double& _resultY)
{
	_resultX = curr_posX;
	_resultY = curr_posY;
}

void MouseController::GetMouseDelta(float& _resultX, float& _resultY)
{
	_resultX = float(curr_posX - prev_posX);
	_resultY = float(curr_posY - prev_posY);
}

void MouseController::GetMouseDelta(double& _resultX, double& _resultY)
{
	_resultX = curr_posX - prev_posX;
	_resultY = curr_posY - prev_posY;
}
