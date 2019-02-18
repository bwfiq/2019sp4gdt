#include "CalamityEarthquake.h"
#include "UIMessagePopup.h"
#include "ConcreteMessages.h"

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
		if (fElapsedTime > fCalamityDuration - fUIPopupTime)
		{
			PO->Send("Scene"
				, new MessageCameraShake(MessageCameraShake::SHAKE_EARTHQUAKE, 0.1f)
			);
			state = STATE_STOPPING;
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