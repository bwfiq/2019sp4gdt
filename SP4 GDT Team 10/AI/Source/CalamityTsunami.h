#pragma once
#include "CalamityBase.h"
#include <vector>

class CalamityTsunami : public CalamityBase
{
public:
	CalamityTsunami();
	~CalamityTsunami();

	virtual void Enter();
	virtual void Update(float dt);
	virtual void Exit();
public:
	enum CALAMITY_TSUNAMI_STATE {
		STATE_NONE,
		STATE_WARNING,
		STATE_SPAWNING,
		STATE_STOPPING,
	};
	CALAMITY_TSUNAMI_STATE state;
	float fUIPopupTime;
	float fWarnTime;
	float fDuration;
	std::vector<unsigned> lanes;
};