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
	enum TIER
	{
		STRAW = 0,
		WOOD,
		STONE,
		FULL_STONE,

		TIER_TOTAL
	};

	STATES eCurrState;
	TIER eCurrTier;

	bool bBuilt;

	virtual bool Handle(Message* msg);

	virtual void TheFunction(GameObject* go);

	Building(GAMEOBJECT_TYPE typeValue = GO_NONE);
	~Building();
};

#endif