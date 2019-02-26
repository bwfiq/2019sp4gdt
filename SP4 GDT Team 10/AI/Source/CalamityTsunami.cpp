#include "CalamityTsunami.h"
#include "UIMessagePopup.h"
#include "ConcreteMessages.h"
#include "SceneData.h"

CalamityTsunami::CalamityTsunami() :
	CalamityBase()
{
}

CalamityTsunami::~CalamityTsunami()
{
}

void CalamityTsunami::Enter()
{
	fUIPopupTime = 4.f;
	fWarnTime = Math::RandFloatMinMax(6, 9);
	fDuration = Math::RandFloatMinMax(9, 12);
	state = STATE_NONE;
	fCalamityDuration = fUIPopupTime + fWarnTime + fDuration;
	UIManager::GetInstance()->AddUI("uiPopupTsunami"
		, new UIMessagePopup("A tsunami's happening!", fUIPopupTime)
	);
	for (int i = 0; i < 4; ++i)
	{
		bool repeated = false;
		unsigned randomLane;
		do
		{
			repeated = false;
			randomLane = Math::RandInt() % SceneData::GetInstance()->GetNoGrid();
			for (auto existingLane : lanes)
			{
				if (randomLane == existingLane) {
					repeated = true;
					break;
				}
			}
		} while (repeated);
		lanes.push_back(randomLane);
	}
}

void CalamityTsunami::Update(float dt)
{
	PostOffice* PO = PostOffice::GetInstance();
	SceneData* SD = SceneData::GetInstance();
	fElapsedTime += dt;
	if (state == STATE_NONE)
	{
		if (fElapsedTime > fUIPopupTime)
		{
			MessageWarnLane* msg = new MessageWarnLane();
			msg->lanes = this->lanes;
			PO->Send("Scene"
				, msg
			);
			state = STATE_WARNING;
		}
	}
	else if (state == STATE_WARNING)
	{
		CSoundEngine::GetInstance()->PlayASound("waves");
		if (fElapsedTime > fUIPopupTime + fWarnTime)
		{
			MessageCalamityTsunami* msg = new MessageCalamityTsunami();
			msg->lanes = this->lanes;
			PO->Send("Scene"
				, msg
			);
			state = STATE_SPAWNING;
		}
	}
	else if (state == STATE_SPAWNING)
	{
		if (fElapsedTime > fCalamityDuration)
		{

			state = STATE_STOPPING;
		}
	}
	else if (state == STATE_STOPPING)
	{
		CSoundEngine::GetInstance()->StopASound("waves");
	}
}

void CalamityTsunami::Exit()
{
	PostOffice::GetInstance()->Send("Scene"
		, new MessageCameraShake(MessageCameraShake::SHAKE_EARTHQUAKE, 0)
	);
}
