#ifndef GAME_ALTAR_H
#define GAME_ALTAR_H

#include "Building.h"

struct Altar : public Building
{
	enum Calamity
	{
		TSUNAMI,
		EARTHQUAKE,
		TORNADO,
		END_OF_THE_WORLD,
		TOTAL
	};
	virtual bool Handle(Message* msg);

	virtual void TheFunction(GameObject* go);
	virtual void Update(float dt);

	void SpawnCalamity(Calamity calamity);

	Altar(GAMEOBJECT_TYPE typeValue = GO_NONE);
	~Altar();

	int iFoodOffered;

	float fCountdown;
	float fMaxCountdown;

	float fGodMad;
	bool bWorldEnded;
};

#endif