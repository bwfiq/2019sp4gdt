#include "CalamityEarthquake.h"
#include "UIMessagePopup.h"
#include "ConcreteMessages.h"
#include "EffectManager.h"

CalamityEarthquake::CalamityEarthquake() :
	CalamityBase()
{
}

CalamityEarthquake::~CalamityEarthquake()
{
}

void CalamityEarthquake::Enter()
{
	fUIPopupTime = 4.f;
	fEffectTimer_Dirt = 0;
	state = STATE_NONE;
	fCalamityDuration = Math::RandFloatMinMax(8, 16) + fUIPopupTime * 2;
	UIManager::GetInstance()->AddUI("uiPopupEarthquake"
		, new UIMessagePopup("An earthquake's happening!", fUIPopupTime)
	);
}

void CalamityEarthquake::Update(float dt)
{
	PostOffice* PO = PostOffice::GetInstance();
	fElapsedTime += dt;
	if (state == STATE_NONE)
	{
		if (fElapsedTime > fUIPopupTime)
		{
			PO->Send("Scene"
				, new MessageCameraShake(MessageCameraShake::SHAKE_EARTHQUAKE, 0.4f)
			);
			PO->Send("Scene"
				, new MessageCalamityEarthquake(50.f, MessageCalamityEarthquake::INTENSE)
			);
			state = STATE_STARTING;
		}
	}
	else if (state == STATE_STARTING)
	{
		if (fElapsedTime > fCalamityDuration * 0.5f)
		{
			PO->Send("Scene"
				, new MessageCameraShake(MessageCameraShake::SHAKE_EARTHQUAKE, 1)
			);
			state = STATE_INTENSE;
		}
	}
	else if (state == STATE_INTENSE)
	{
		fEffectTimer_Dirt += dt;
		if (fElapsedTime > fCalamityDuration - fUIPopupTime)
		{
			PO->Send("Scene"
				, new MessageCameraShake(MessageCameraShake::SHAKE_EARTHQUAKE, 0.1f)
			);
			PO->Send("Scene"
				, new MessageCalamityEarthquake(0.f, MessageCalamityEarthquake::STOPPING)
			);
			state = STATE_STOPPING;
		}
		if (fEffectTimer_Dirt > 0.06f)
		{
			fEffectTimer_Dirt = 0;
			EffectManager::GetInstance()->DoPrefabEffect(EffectManager::PREFAB_EARTHQUAKE_DEBRIS);
		}
	}
	else if (state == STATE_STOPPING)
	{

	}
}

void CalamityEarthquake::Exit()
{
	PostOffice::GetInstance()->Send("Scene"
		, new MessageCameraShake(MessageCameraShake::SHAKE_EARTHQUAKE, 0)
	);
}
