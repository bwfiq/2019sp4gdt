#include "CalamityMeteorShower.h"
#include "UIMessagePopup.h"
#include "ConcreteMessages.h"
#include "EffectManager.h"
#include "Grid.h"
#include "SceneData.h"

CalamityMeteorShower::CalamityMeteorShower() :
	CalamityBase(),
	state(STATE_NONE),
	fUIPopupTime(0)
{
}

CalamityMeteorShower::~CalamityMeteorShower()
{
}

void CalamityMeteorShower::Enter()
{
	fUIPopupTime = 4.f;
	state = STATE_NONE;
	fCalamityDuration = Math::RandFloatMinMax(8, 16) + fUIPopupTime + 4;
	int noGrid = SceneData::GetInstance()->GetNoGrid() - 1;
	this->meteorGridPts.clear();
	for (int i = 0; i < Math::RandIntMinMax(noGrid * 0.45f, noGrid * 0.75f); ++i)
	{
		GridPt newGrid;
		bool hasGridInVector = false;
		do
		{
			newGrid = GridPt(Math::RandIntMinMax(0, noGrid), Math::RandIntMinMax(0, noGrid));
			hasGridInVector = false;
			for (auto otherGrid : this->meteorGridPts)
			{
				if (otherGrid == newGrid)
				{
					hasGridInVector = true;
				}
			}
		} while (hasGridInVector);
		this->meteorGridPts.push_back(newGrid);
	}
	UIManager::GetInstance()->AddUI("uiPopupMeteorShower"
		, new UIMessagePopup("A meteor shower's occuring!", fUIPopupTime)
	);
}

void CalamityMeteorShower::Update(float dt)
{
	PostOffice* PO = PostOffice::GetInstance();
	fElapsedTime += dt;
	if (state == STATE_NONE)
	{
		if (fElapsedTime > fUIPopupTime)
		{
			PO->Send("Scene",
				new MessageWarnGrids(this->meteorGridPts));
			state = STATE_WARNING;
		}
	}
	else if (state == STATE_WARNING)
	{
		if (fElapsedTime > fUIPopupTime + 4)
		{
			PO->Send("Scene",
				new MessageCalamityMeteorShower(this->meteorGridPts));
			state = STATE_SHOWER;
		}
	}
	else if (state == STATE_SHOWER)
	{
		if (fElapsedTime > fCalamityDuration)
		{
			MessageCalamityEnd* msg = new MessageCalamityEnd();
			PO->Send("Scene"
				, msg
			);
			state = STATE_STOPPING;
		}
	}
	else if (state == STATE_STOPPING)
	{
	}
}

void CalamityMeteorShower::Exit()
{
	PostOffice::GetInstance()->Send("Scene"
		, new MessageCameraShake(MessageCameraShake::SHAKE_EARTHQUAKE, 0)
	);
}
