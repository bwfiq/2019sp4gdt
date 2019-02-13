#ifndef GAME_WOOD_H
#define GAME_WOOD_H

#include "Environment.h"

struct Tree : public Environment
{

	enum STATES
	{
		FULL = 0,
		HALFCHOPPED,
		DEPLETED,
		STATES_TOTAL //Should not have any enums below this
	};

	STATES eCurrState; //State for rendering as well as efficiency / other gameplay features

	virtual bool Handle(Message* msg);

	float fTimer;

	int iWoodAmount;

	//virtual void TheFunction(GameObject* go);

	Tree(GAMEOBJECT_TYPE typeValue = GO_NONE);
	~Tree();
};

#endif