#include "CalamityBase.h"

class CalamityTornado : public CalamityBase
{
public:
	CalamityTornado();
	~CalamityTornado();

	virtual void Enter();
	virtual void Update(float dt);
	virtual void Exit();
private:
	enum CALAMITY_TORNADO_STATE {
		STATE_NONE,
		STATE_STARTING,
		STATE_STOPPING
	};
	CALAMITY_TORNADO_STATE state;
	float fUIPopupTime;
	float fEffectTimer_Dirt;
};