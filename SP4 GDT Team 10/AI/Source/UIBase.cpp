#include "UIBase.h"
#include "Mesh.h"
#include "MouseController.h"
#include "Application.h"
#include "UITween.h"

UIBase::UIBase() :
	bLightEnabled(false),
	bActive(true),
	fRotation(0),
	scale(1, 1, 1),
	bIsDone(false),
	bIsMouseHovered(false),
	currentTween(NULL)
{
}

UIBase::~UIBase()
{
	if (currentTween != NULL)
	{
		delete currentTween;
		currentTween = NULL;
	}
	while (!tween_queue.empty()) {
		delete tween_queue.front();
		tween_queue.pop();
	}
}

void UIBase::UpdateTween(float dt)
{
	if (currentTween == NULL)
	{
		if (!tween_queue.empty())
		{
			currentTween = tween_queue.front();
			tween_queue.pop();
		}
	}
	else
	{
		currentTween->Update(dt);
		if (currentTween->IsDone())
		{
			delete currentTween;
			currentTween = NULL;
			if (!tween_queue.empty())
			{
				currentTween = tween_queue.front();
				tween_queue.pop();
			}
		}
	}
}

void UIBase::AddTween(UITween* uiTween)
{
	if (uiTween != NULL)
		tween_queue.push(uiTween);
}

UITween* UIBase::GetTween()
{
	return currentTween;
}

void UIBase::SetTweenDone()
{
	if(currentTween != NULL)
		currentTween->SetIsDone(true);
}

bool UIBase::IsMouseHovered()
{
	if (scale.x <= 0 || scale.y <= 0) return false;//if the ui is even that small then dont bother at all
	float mouseX, mouseY;
	MouseController::GetInstance()->GetMousePosition(mouseX, mouseY);
	int windowWidth, windowHeight;
	windowWidth = Application::GetInstance().GetWindowWidth();
	windowHeight = Application::GetInstance().GetWindowHeight();
	if (mouseX < 0 || mouseX > windowWidth ||
		mouseY < 0 || mouseY > windowHeight)
		return false;//checks if the mouse is even within window boundaries
	mouseY = windowHeight - mouseY;
	//std::cout << mouseX << " " << mouseY << std::endl;
	float centerX, centerY;//center of the UIComponent
	/* I WAN 2 DIE
		~Calculates the uiBase's center first when it's anchorPoint is at 0.5,0.5 ;
		~Then it adjusts it so it matches according to the uiBase's anchorPoint ;
		
	*/
	centerX = (pos.x * windowWidth)
		+ (scale.x * (0.5f - anchorPoint.x));
	centerY = (pos.y * windowHeight)
		+ ((0.5f - anchorPoint.y) * scale.y);

	//std::cout << centerX << " " << centerY << std::endl;
	float minX, minY, maxX, maxY;
	minX = centerX - scale.x * 0.5f;
	minY = centerY - scale.y * 0.5f;
	maxX = centerX + scale.x * 0.5f;
	maxY = centerY + scale.y * 0.5f;
	//std::cout << minX << " " << maxX << " " << minY << " " << maxY << std::endl;
	if (mouseX > minX && mouseX < maxX &&
		mouseY > minY && mouseY < maxY)
	{
		return true;
	}

	return false;
}

bool UIBase::IsMousePressed()
{
	return MouseController::GetInstance()->IsButtonPressed(MouseController::LMB) && this->IsMouseHovered();
}
