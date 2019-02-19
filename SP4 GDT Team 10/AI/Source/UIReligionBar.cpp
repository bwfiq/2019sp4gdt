#include "UIReligionBar.h"
#include "SceneData.h"

UIReligionBar::UIReligionBar() :
	UIBase()
{
	SceneData* SD = SceneData::GetInstance();
	//UIComponent UIC;
	//UIC.mesh = SD->GetMesh("whitequad");
	//uiComponents_list.push_back(UIC);
	//UIC.mesh = SD->GetMesh("greyquad");
	//uiComponents_list.push_back(UIC);

	//UIC.mesh = SD->GetMesh("redquad");
	//uiComponents_list.push_back(UIC);
	////UIC.mesh = SD->GetMesh("yellowquad");
	////uiComponents_list.push_back(UIC);
	////UIC.mesh = SD->GetMesh("greenquad");
	////uiComponents_list.push_back(UIC);

	//UIC.mesh = SD->GetMesh("blueTriangle");
	//uiComponents_list.push_back(UIC);

	uiComponents_list.resize(COMPONENT_TOTAL);
	uiComponents_list[COMPONENT_OUTLINEBAR].mesh = SD->GetMesh("whitequad");
	uiComponents_list[COMPONENT_GREYBAR].mesh = SD->GetMesh("greyquad");
	uiComponents_list[COMPONENT_THEBAR].mesh = SD->GetMesh("redquad");
	uiComponents_list[COMPONENT_FIRSTTOP].mesh = SD->GetMesh("blueTriangle");
	uiComponents_list[COMPONENT_FIRSTBOT].mesh = SD->GetMesh("blueTriangle");
	uiComponents_list[COMPONENT_SECONDTOP].mesh = SD->GetMesh("blueTriangle");
	uiComponents_list[COMPONENT_SECONDBOT].mesh = SD->GetMesh("blueTriangle");

	pos.Set(0.5f, 0.9f);
	scale.Set(600, 40);
	anchorPoint.Set(0.5, 0.5);
	float ratio = scale.x / scale.y;
	
	uiComponents_list[COMPONENT_OUTLINEBAR].scale.Set(1 + 0.2f / ratio, 1.2f);
	uiComponents_list[COMPONENT_OUTLINEBAR].anchorPoint.Set(0, 0);

	//uiComponents_list[COMPONENT_GREYBAR].pos.Set(1.f/3.f, 0);
	//uiComponents_list[COMPONENT_GREYBAR].anchorPoint.Set(0, 0);
	//uiComponents_list[COMPONENT_GREYBAR].scale.Set(1.f / 3.f, 1);

	uiComponents_list[COMPONENT_GREYBAR].pos.Set(0, 0);
	uiComponents_list[COMPONENT_GREYBAR].anchorPoint.Set(0, 0);
	uiComponents_list[COMPONENT_GREYBAR].scale.Set(1, 1);
	//uiComponents_list[COMPONENT_GREYBAR].text = "test";
	//uiComponents_list[COMPONENT_GREYBAR].textSize = scale.y;

	//uiComponents_list[COMPONENT_REDBAR].pos.Set(0, 0);
	//uiComponents_list[COMPONENT_REDBAR].anchorPoint.Set(0, 0);
	//uiComponents_list[COMPONENT_REDBAR].scale.Set(1.f / 3.f, 1);

	//uiComponents_list[COMPONENT_YELLOWBAR].pos.Set(0, 0);
	//uiComponents_list[COMPONENT_YELLOWBAR].anchorPoint.Set(0, 0);
	//uiComponents_list[COMPONENT_YELLOWBAR].scale.Set(0, 1);
	//uiComponents_list[COMPONENT_YELLOWBAR].alpha = 0.4f;

	//uiComponents_list[COMPONENT_GREENBAR].pos.Set(2.f/3.f, 0);
	//uiComponents_list[COMPONENT_GREENBAR].anchorPoint.Set(0, 0);
	//uiComponents_list[COMPONENT_GREENBAR].scale.Set(1.f / 3.f, 1);

	uiComponents_list[COMPONENT_THEBAR].pos.Set(0, 0);
	uiComponents_list[COMPONENT_THEBAR].anchorPoint.Set(0, 0);
	uiComponents_list[COMPONENT_THEBAR].scale.Set(0, 1);
	uiComponents_list[COMPONENT_THEBAR].alpha = 0.9f;


	uiComponents_list[COMPONENT_FIRSTTOP].pos.Set(0.333333f, 1);
	uiComponents_list[COMPONENT_FIRSTTOP].fRotation = 180;
	uiComponents_list[COMPONENT_FIRSTTOP].scale.Set(0.025f, 0.5f);
	uiComponents_list[COMPONENT_FIRSTTOP].anchorPoint.Set(0.5f, 0.5f);

	uiComponents_list[COMPONENT_FIRSTBOT].pos.Set(0.333333f, 0);
	uiComponents_list[COMPONENT_FIRSTBOT].fRotation = 0;
	uiComponents_list[COMPONENT_FIRSTBOT].scale.Set(0.025f, 0.5f);
	uiComponents_list[COMPONENT_FIRSTBOT].anchorPoint.Set(0.5f, 0.5f);

	uiComponents_list[COMPONENT_SECONDTOP].pos.Set(0.666666f, 1);
	uiComponents_list[COMPONENT_SECONDTOP].fRotation = 180;
	uiComponents_list[COMPONENT_SECONDTOP].scale.Set(0.025f, 0.5f);
	uiComponents_list[COMPONENT_SECONDTOP].anchorPoint.Set(0.5f, 0.5f);

	uiComponents_list[COMPONENT_SECONDBOT].pos.Set(0.666666f, 0);
	uiComponents_list[COMPONENT_SECONDBOT].fRotation = 0;
	uiComponents_list[COMPONENT_SECONDBOT].scale.Set(0.025f, 0.5f);
	uiComponents_list[COMPONENT_SECONDBOT].anchorPoint.Set(0.5f, 0.5f);
}

UIReligionBar::~UIReligionBar()
{
}

void UIReligionBar::Update(float dt)
{
	SceneData* SD = SceneData::GetInstance();

	float fReligionValue = SceneData::GetInstance()->GetReligionValue();

	//uiComponents_list[COMPONENT_BLUETRIANGLE].pos.Set(
	//	Math::lerp(uiComponents_list[COMPONENT_BLUETRIANGLE].pos.x, fReligionValue//fReligionValue / 100.f
	//		, Math::Min((float)dt * 10.f,1.f))
	//		, 0);

	uiComponents_list[COMPONENT_THEBAR].scale.Set(
		Math::lerp(uiComponents_list[COMPONENT_THEBAR].scale.x, fReligionValue / SceneData::GetInstance()->GetMaxReligionValue()
			, Math::Min((float)dt * 10.f, 1.f))
			, 1, 1);

	if (uiComponents_list[COMPONENT_THEBAR].scale.x < 0.333333f)
	{
		uiComponents_list[COMPONENT_THEBAR].mesh = SD->GetMesh("redquad");
	}
	else if (uiComponents_list[COMPONENT_THEBAR].scale.x < 0.666666f)
	{
		uiComponents_list[COMPONENT_THEBAR].mesh = SD->GetMesh("yellowquad");
	}
	else
	{
		uiComponents_list[COMPONENT_THEBAR].mesh = SD->GetMesh("greenquad");
	}
}
