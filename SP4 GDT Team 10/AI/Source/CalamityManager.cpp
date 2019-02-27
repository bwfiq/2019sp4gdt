#include "CalamityManager.h"
#include "CalamityBase.h"

void CalamityManager::Init()
{
	currCalamity = NULL;
}

void CalamityManager::Update(float dt)
{
	if (currCalamity != NULL)
	{
		currCalamity->Update(dt);
		if (currCalamity->IsDone())
			SwitchToNextCalamity();
	}
	else if (currCalamity == NULL && !calamityQueue.empty())
		SwitchToNextCalamity();
}

void CalamityManager::AddToCalamityQueue(CalamityBase * cal)
{
	calamityQueue.push(cal);
}

CalamityBase * CalamityManager::GetCurrCalamity()
{
	return this->currCalamity;
}

std::queue<CalamityBase*> CalamityManager::GetCalamityQueue()
{
	return this->calamityQueue;
}

CalamityManager::CalamityManager()
{

}

CalamityManager::~CalamityManager()
{

}

void CalamityManager::SwitchToNextCalamity()
{
	if (currCalamity != NULL)
	{
		currCalamity->Exit();
		CSoundEngine::GetInstance()->StopASound("rumble");
		delete currCalamity;
	}
	if (calamityQueue.empty())
	{
		currCalamity = NULL;
	}
	else
	{
		currCalamity = calamityQueue.front();
		calamityQueue.pop();
		CSoundEngine::GetInstance()->PlayASound("rumble");
		currCalamity->Enter();
	}
}
