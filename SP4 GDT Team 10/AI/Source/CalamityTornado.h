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
	enum CALAMITY_EARTHQUAKE_STATE {
		STATE_NONE,
		STATE_STARTING,
		STATE_STOPPING
	};

	CALAMITY_EARTHQUAKE_STATE state;
	float fUIPopupTime;
	float fEffectTimer_Dirt;
};