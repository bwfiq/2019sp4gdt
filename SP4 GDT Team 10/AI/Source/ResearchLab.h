#ifndef GAME_RESEARCHLAB_H
#define GAME_RESEARCHLAB_H

#include "Building.h"

struct ResearchLab : public Building
{

	virtual bool Handle(Message* msg);

	virtual void TheFunction(GameObject* go);

	ResearchLab(GAMEOBJECT_TYPE typeValue = GO_NONE);
	~ResearchLab();
};

#endif