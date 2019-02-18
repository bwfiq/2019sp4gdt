#ifndef GAME_MOUNTAIN_H
#define GAME_MOUNTAIN_H

#include "Environment.h"

struct Mountain : public Environment
{

	//enum STATES
	//{
	//	FULL = 0,
	//	HALF_MINED,
	//	//DEPLETED,

	//	STATES_TOTAL //Should not have any enums below this
	//};

//	STATES eCurrState; //State for rendering as well as efficiency / other gameplay features

	virtual bool Handle(Message* msg);

	int iStoneGain;
	int iStoneAmount;

	//virtual void TheFunction(GameObject* go);

	Mountain(GAMEOBJECT_TYPE typeValue = GO_NONE);
	~Mountain();
};

#endif