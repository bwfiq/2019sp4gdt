#ifndef GAME_BUSH_H
#define GAME_BUSH_H

#include "GameObject.h"

struct Bush : public GameObject
{

	enum STATES
	{
		LUSH = 0,
		DEPLETED,

		STATES_TOTAL //Should not have any enums below this
	};

	STATES eCurrState; //State for rendering as well as efficiency / other gameplay features

	virtual bool Handle(Message* msg);

	void DoFunction();

	//virtual void TheFunction(GameObject* go);

	Bush(GAMEOBJECT_TYPE typeValue = GO_NONE);
	~Bush();
};

#endif