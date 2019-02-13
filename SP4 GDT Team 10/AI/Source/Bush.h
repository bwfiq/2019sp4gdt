#ifndef GAME_BUSH_H
#define GAME_BUSH_H

#include "Environment.h"

struct Bush : public Environment
{

	enum STATES
	{
		LUSH = 0,
		DEPLETED,

		STATES_TOTAL //Should not have any enums below this
	};

	STATES eCurrState; //State for rendering as well as efficiency / other gameplay features

	virtual bool Handle(Message* msg);

	float fTimer;

	int foodAmount;

	//virtual void TheFunction(GameObject* go);

	Bush(GAMEOBJECT_TYPE typeValue = GO_NONE);
	~Bush();
};

#endif