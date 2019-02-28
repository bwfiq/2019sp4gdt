#pragma once
#include "SingletonTemplate.h"
#include <queue>

class CalamityBase;

class CalamityManager : public Singleton<CalamityManager>
{
	friend Singleton<CalamityManager>;
public:
	void Init();
	void Update(float dt);

	void AddToCalamityQueue(CalamityBase* cal);
	void ClearAllCalamities();

	CalamityBase* GetCurrCalamity();
	std::queue<CalamityBase*> GetCalamityQueue();
private:
	CalamityManager();
	~CalamityManager();

	CalamityBase* currCalamity;
	std::queue<CalamityBase*> calamityQueue;

	void SwitchToNextCalamity();
};

