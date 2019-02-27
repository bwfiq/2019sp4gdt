#include "CalamityTornado.h"
#include "UIMessagePopup.h"
#include "ConcreteMessages.h"
#include "EffectManager.h"

CalamityTornado::CalamityTornado() :
	CalamityBase(),
	state(STATE_NONE),
	fUIPopupTime(0),
	fEffectTimer_Dirt(0)
{
}

CalamityTornado::~CalamityTornado()
{
}

void CalamityTornado::Enter()
{
	fUIPopupTime = 4.f;
	fEffectTimer_Dirt = 0;
	state = STATE_NONE;
	fCalamityDuration = Math::RandFloatMinMax(8, 16) + fUIPopupTime * 2;
	UIManager::GetInstance()->AddUI("uiPopupTornado"
		, new UIMessagePopup("A tornado's forming!", fUIPopupTime)
	);
}

void CalamityTornado::Update(float dt)
{
	PostOffice* PO = PostOffice::GetInstance();
	fElapsedTime += dt;
	if (state == STATE_NONE)
	{
		if (fElapsedTime > fUIPopupTime)
		{
			PO->Send("Scene",
				new MessageCalamityTornado(100)
			);
			state = STATE_STARTING;
		}
	}
	else if (state == STATE_STARTING)
	{
		if (fElapsedTime > fCalamityDuration)
		{

			state = STATE_STOPPING;
		}
	}
	else if (state == STATE_STOPPING)
	{

	}
}

void CalamityTornado::Exit()
{
}