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
		currCalamity->Enter();
	}
}