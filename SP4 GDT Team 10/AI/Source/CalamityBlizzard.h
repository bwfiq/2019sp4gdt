#include "CalamityBase.h"

class CalamityBlizzard : public CalamityBase
{
public:
	CalamityBlizzard();
	~CalamityBlizzard();

	virtual void Enter();
	virtual void Update(float dt);
	virtual void Exit();
public:
	enum CALAMITY_BLIZZARD_STATE {
		STATE_NONE,
		STATE_STARTING,
		STATE_INTENSE,
		STATE_STOPPING
	};
	CALAMITY_BLIZZARD_STATE state;
	float fUIPopupTime;
	float fEffectTimer_BlizzardCloud;
};