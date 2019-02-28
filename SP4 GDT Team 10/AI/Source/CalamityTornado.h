#include "CalamityBase.h"

class CalamityTornado : public CalamityBase
{
public:
	CalamityTornado();
	~CalamityTornado();

	virtual void Enter();
	virtual void Update(float dt);
	virtual void Exit();
public:

	enum CALAMITY_TORNADO_STATE {
		STATE_NONE,
		STATE_STARTING,
		STATE_STOPPING,
		STATE_TOTAL
	};
	CALAMITY_TORNADO_STATE state;
	float fUIPopupTime;
	float fEffectTimer_Dirt;
};