#ifndef GAME_STONESHED_H
#define GAME_STONESHED_H

#include "Building.h"

struct StoneShed : public Building
{

	virtual bool Handle(Message* msg);

	virtual void TheFunction(GameObject* go);

	int stoneCapacity;

	StoneShed(GAMEOBJECT_TYPE typeValue = GO_NONE);
	~StoneShed();
};

#endif