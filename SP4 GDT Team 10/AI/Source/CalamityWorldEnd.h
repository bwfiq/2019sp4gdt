#include "CalamityBase.h"

class CalamityWorldEnd : public CalamityBase
{
public:
	CalamityWorldEnd();
	~CalamityWorldEnd();

	virtual void Enter();
	virtual void Update(float dt);
	virtual void Exit();
public:
	float fUIPopupTime;
	float fEffectTimer_Dirt;
};