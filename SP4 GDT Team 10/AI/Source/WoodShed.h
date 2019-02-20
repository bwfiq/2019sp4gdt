#ifndef GAME_WOODSHED_H
#define GAME_WOODSHED_H

#include "Building.h"

struct WoodShed : public Building
{

	virtual bool Handle(Message* msg);

	virtual void TheFunction(GameObject* go);

	int woodCapacity;

	WoodShed(GAMEOBJECT_TYPE typeValue = GO_NONE);
	~WoodShed();
};

#endif