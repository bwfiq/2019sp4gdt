#pragma once
#include "PostOffice.h"
#include "UIManager.h"

class CalamityBase {
public:
	CalamityBase();
	virtual ~CalamityBase();

	bool bActive;
	float fElapsedTime;
	float fCalamityDuration;
	virtual void Enter() = 0;
	virtual void Update(float dt) = 0;
	virtual void Exit() = 0;
	virtual bool IsDone();
protected:
private:
	//virtual void Render() = 0;
};