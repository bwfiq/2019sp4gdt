#ifndef GAME_GRANARY_H
#define GAME_GRANARY_H

#include "Building.h"

struct Granary : public Building
{

	virtual bool Handle(Message* msg);

	virtual void TheFunction(GameObject* go);

	int foodCapacity;

	Granary(GAMEOBJECT_TYPE typeValue = GO_NONE);
	~Granary();
};

#endif