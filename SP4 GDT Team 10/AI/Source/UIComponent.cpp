#include "UIComponent.h"
#include "Mesh.h"
#include "UIBase.h"
#include "SceneData.h"
#include "Application.h"
#include "MouseController.h"

UIComponent::UIComponent() : 
	fRotation(0)
	, bActive(true)
	, mesh(NULL)
	, scale(1, 1, 1)
	, type(UI_FRAME)
{
}

UIComponent::~UIComponent()
{
}

//bool UIComponent::IsMouseHovered(UIBase * uiBase)
//{
//	if (scale.x <= 0 || scale.y <= 0 || uiBase->scale.x <= 0 || uiBase->scale.y <= 0) return false;//if the ui is even that small then dont bother at all
//	float mouseX, mouseY;
//	MouseController::GetInstance()->GetMousePosition(mouseX, mouseY);
//	int windowWidth, windowHeight;
//	windowWidth = Application::GetInstance().GetWindowWidth();
//	windowHeight = Application::GetInstance().GetWindowHeight();
//	if (mouseX < 0 || mouseX > windowWidth ||
//		mouseY < 0 || mouseY > windowHeight)
//		return false;//checks if the mouse is even within window boundaries
//	mouseY = windowHeight - mouseY;
//	//std::cout << mouseX << " " << mouseY << std::endl;
//	float centerX, centerY;//center of the UIComponent
//	/* I WAN 2 DIE
//		~Calculates the uiBase's center first when it's anchorPoint is at 0.5,0.5 ;
//		~Then it adjusts it so it matches according to the uiBase's anchorPoint ;
//		!!!!!! THIS THING ONLY CALCULATES THE MOUSE IN THE UIBASE, NOT THE INDIVIDUAL COMPONENT !!!!!!
//	*/
//	centerX = (uiBase->pos.x * windowWidth)
//		+ (uiBase->scale.x * (0.5f - uiBase->anchorPoint.x));
//	centerY = (uiBase->pos.y * windowHeight)
//		+ ((0.5f - uiBase->anchorPoint.y) * uiBase->scale.y);
//			
//	//std::cout << centerX << " " << centerY << std::endl;
//	float minX, minY, maxX, maxY;
//	minX = centerX - uiBase->scale.x * 0.5f;
//	minY = centerY - uiBase->scale.y * 0.5f;
//	maxX = centerX + uiBase->scale.x * 0.5f;
//	maxY = centerY + uiBase->scale.y * 0.5f;
//	//std::cout << minX << " " << maxX << " " << minY << " " << maxY << std::endl;
//	if (mouseX > minX && mouseX < maxX &&
//		mouseY > minY && mouseY < maxY)
//	{
//		return true;
//	}
//
//	return false;
//}
