#ifndef GAME_CHIEFHUT_H
#define GAME_CHIEFHUT_H

#include "Building.h"

struct ChiefHut : public Building
{
	int iHousingSpace;
	virtual bool Handle(Message* msg);

	virtual void TheFunction(GameObject* go);

	ChiefHut(GAMEOBJECT_TYPE typeValue = GO_NONE);
	~ChiefHut();
};

#endif