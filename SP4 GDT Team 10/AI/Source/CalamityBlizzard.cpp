#include "CalamityBlizzard.h"
#include "UIMessagePopup.h"
#include "ConcreteMessages.h"
#include "EffectManager.h"

CalamityBlizzard::CalamityBlizzard() :
	CalamityBase(),
	state(STATE_NONE),
	fUIPopupTime(0),
	fEffectTimer_BlizzardCloud(0)
{
}

CalamityBlizzard::~CalamityBlizzard()
{
}

void CalamityBlizzard::Enter()
{
	fUIPopupTime = 4.f;
	fEffectTimer_BlizzardCloud = 0;
	state = STATE_NONE;
	fCalamityDuration = Math::RandFloatMinMax(8, 16) + fUIPopupTime * 2;
	UIManager::GetInstance()->AddUI("uiPopupBlizzard"
		, new UIMessagePopup("A blizzard's coming!", fUIPopupTime)
	);
}

void CalamityBlizzard::Update(float dt)
{
	PostOffice* PO = PostOffice::GetInstance();
	fElapsedTime += dt;
	if (state == STATE_NONE)
	{
		if (fElapsedTime > fUIPopupTime)
		{
			PO->Send("Scene"
				, new MessageCalamityBlizzard(50.f, MessageCalamityBlizzard::INTENSE)
			);
			state = STATE_STARTING;
		}
	}
	else if (state == STATE_STARTING)
	{
		fEffectTimer_BlizzardCloud += dt;
		if (fElapsedTime > fCalamityDuration * 0.5f)
		{
			
			state = STATE_INTENSE;
		}
		if (fEffectTimer_BlizzardCloud > 0.19f)
		{
			fEffectTimer_BlizzardCloud = 0;
			EffectManager::GetInstance()->DoPrefabEffect(EffectManager::PREFAB_BLIZZARD_CLOUDS);
		}
	}
	else if (state == STATE_INTENSE)
	{
		
		if (fElapsedTime > fCalamityDuration - fUIPopupTime)
		{
			PO->Send("Scene"
				, new MessageCalamityBlizzard(0.f, MessageCalamityBlizzard::STOPPING)
			);
			state = STATE_STOPPING;
		}
		
	}
	else if (state == STATE_STOPPING)
	{

	}
}

void CalamityBlizzard::Exit()
{
	PostOffice::GetInstance()->Send("Scene"
		, new MessageCameraShake(MessageCameraShake::SHAKE_EARTHQUAKE, 0)
	);
}
