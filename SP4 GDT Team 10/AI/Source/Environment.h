#ifndef GAME_ENVIRONMENT_H
#define GAME_ENVIRONMENT_H

#include "GameObject.h"

struct Environment : public GameObject
{

	virtual bool Handle(Message* msg);

	float fTimer;

	//virtual void TheFunction(GameObject* go);

	Environment(GAMEOBJECT_TYPE typeValue = GO_NONE);
	~Environment();
};

#endif