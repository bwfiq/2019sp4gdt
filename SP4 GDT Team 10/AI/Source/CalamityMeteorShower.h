#pragma once
#include "CalamityBase.h"

struct GridPt;

class CalamityMeteorShower : public CalamityBase
{
public:
	CalamityMeteorShower();
	~CalamityMeteorShower();

	virtual void Enter();
	virtual void Update(float dt);
	virtual void Exit();
public:
	enum CALAMITY_METEORSHOWER_STATE {
		STATE_NONE,
		STATE_WARNING,
		STATE_SHOWER,
		STATE_STOPPING,
	};
	CALAMITY_METEORSHOWER_STATE state;
	float fUIPopupTime;
	std::vector<GridPt> meteorGridPts;
};