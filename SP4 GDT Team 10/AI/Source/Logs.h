#ifndef GAME_LOGS_H
#define GAME_LOGS_H

#include "Building.h"

struct Logs : public Building
{

	virtual bool Handle(Message* msg);

	virtual void TheFunction(GameObject* go);

	Logs(GAMEOBJECT_TYPE typeValue = GO_NONE);
	~Logs();
};

#endif