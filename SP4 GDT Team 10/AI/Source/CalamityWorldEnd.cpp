#include "CalamityWorldEnd.h"
#include "UIMessagePopup.h"
#include "ConcreteMessages.h"
#include "EffectManager.h"
#include "SceneData.h"

CalamityWorldEnd::CalamityWorldEnd() :
	CalamityBase(),
	fUIPopupTime(0),
	fEffectTimer_Dirt(0)
{
}

CalamityWorldEnd::~CalamityWorldEnd()
{
}

void CalamityWorldEnd::Enter()
{
	fUIPopupTime = 4.f;
	fEffectTimer_Dirt = 0;
	fCalamityDuration = Math::RandFloatMinMax(8, 16) + fUIPopupTime * 2;
	UIManager::GetInstance()->AddUI("uiPopupWorldEnd"
		, new UIMessagePopup("The World is Ending!", fUIPopupTime)
	);
}

void CalamityWorldEnd::Update(float dt)
{
	PostOffice* PO = PostOffice::GetInstance();
	fElapsedTime += dt;
	PO->Send("Scene"
		, new MessageCalamityWorldEnd()
	);
	fEffectTimer_Dirt += dt;
	if (fEffectTimer_Dirt > 0.2f)
	{
		float worldLength = SceneData::GetInstance()->GetGridSize() * SceneData::GetInstance()->GetNoGrid() * 0.5f;
		EffectManager::GetInstance()->DoPrefabEffect(EffectManager::PREFAB_PLACEOBJECT, Vector3(Math::RandFloatMinMax(-worldLength, worldLength), 0.2f, Math::RandFloatMinMax(-worldLength, worldLength)));
	}
}

void CalamityWorldEnd::Exit()
{
}
