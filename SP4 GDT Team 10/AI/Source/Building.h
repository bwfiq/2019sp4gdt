#ifndef GAME_BUILDING_H
#define GAME_BUILDING_H

#include "GameObject.h"

struct Building : public GameObject
{
	/*
	bool moveLeft;
	bool moveRight;
	bool moveUp;
	bool moveDown;*/

	enum STATES
	{
		COMPLETED = 0,
		CONSTRUCTING,
		BROKEN,
		BLUEPRINT,
		STATES_TOTAL //Should not have any enums below this
	};

	STATES eCurrState;

	virtual bool Handle(Message* msg);

	Building(GAMEOBJECT_TYPE typeValue = GO_NONE);
	~Building();
};

#endif