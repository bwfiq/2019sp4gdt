#ifndef GAME_BUILDING_H
#define GAME_BUILDING_H

#include "GameObject.h"

struct Building : public GameObject
{

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

	virtual void TheFunction(GameObject* go);

	Building(GAMEOBJECT_TYPE typeValue = GO_NONE);
	~Building();
};

#endif