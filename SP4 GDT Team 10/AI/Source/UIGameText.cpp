#include "UIGameText.h"
#include "SceneData.h"
#include "PostOffice.h"
#include "ConcreteMessages.h"

UIGameText::UIGameText(TEXT_TYPE textType) :
	UIBase()
{
	SceneData* SD = SceneData::GetInstance();
	uiComponents_list.resize(COMPONENT_TOTAL);
	switch (textType)
	{
	case TEXT_DAILYREQUIREMENT:
		pos.Set(0, 0.6);
		scale.Set(225, 250);
		anchorPoint.Set(0, 1);
		uiComponents_list[COMPONENT_BOARD].mesh = SD->GetMesh("ui_board_blank");
		for (int i = 0; i <= COMPONENT_TEXT_3 - COMPONENT_TEXT_1; ++i)
		{
			uiComponents_list[COMPONENT_TEXT_1 + i].mesh = NULL;
			uiComponents_list[COMPONENT_TEXT_1 + i].text = "asdasdasd";
			uiComponents_list[COMPONENT_TEXT_1 + i].textSize = scale.y * 0.1f;
			uiComponents_list[COMPONENT_TEXT_1 + i].pos.Set(0.65f, (i / 3.f));//very hardcoded anchorpoint offset
			uiComponents_list[COMPONENT_TEXT_1 + i].scale.Set(1, 0.333f);
			uiComponents_list[COMPONENT_TEXT_1 + i].anchorPoint.Set(0, 1);
		}
		break;
	}
	this->textType = textType;
	
	float ratio = scale.x / scale.y;
}

UIGameText::~UIGameText()
{
}

void UIGameText::Update(float dt)
{
}

void UIGameText::SetText(std::string text)
{
}
