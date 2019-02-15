#ifndef GAME_ALTAR_H
#define GAME_ALTAR_H

#include "Building.h"

struct Altar : public Building
{
	virtual bool Handle(Message* msg);

	virtual void TheFunction(GameObject* go);

	Altar(GAMEOBJECT_TYPE typeValue = GO_NONE);
	~Altar();

	int iFoodOffered;
};

#endif