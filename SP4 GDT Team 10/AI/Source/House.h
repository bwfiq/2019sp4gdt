#ifndef GAME_HOUSE_H
#define GAME_HOUSE_H

#include "Building.h"

struct House : public Building
{
	int iHousingSpace;

	virtual bool Handle(Message* msg);

	virtual void TheFunction(GameObject* go);

	House(GAMEOBJECT_TYPE typeValue = GO_NONE);
	~House();
};

#endif