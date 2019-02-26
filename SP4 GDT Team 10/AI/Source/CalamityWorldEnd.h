#include "CalamityBase.h"

class CalamityWorldEnd : public CalamityBase
{
public:
	CalamityWorldEnd();
	~CalamityWorldEnd();

	virtual void Enter();
	virtual void Update(float dt);
	virtual void Exit();
private:
	float fUIPopupTime;
	float fEffectTimer_Dirt;
};