#ifndef GAME_VILLAGER_H
#define GAME_VILLAGER_H

#include "GameObject.h"

struct Villager : public GameObject
{
	/*
	bool moveLeft;
	bool moveRight;
	bool moveUp;
	bool moveDown;*/

	enum STATS
	{
		HUNTING = 0,
		FORAGING,
		WOODCUTTING,
		BUILDING,
		BREEDING,
		COMBAT,
		STAT_TOTAL
	};

	enum STATES
	{
		HEALTHY = 0,
		TIRED,
		SICKLY,
		DYING,
		STATES_TOTAL //Should not have any enums below this
	};

	float fStats[STAT_TOTAL]; //Stats for the efficiency of doing different activities, 1.0f is normal, can go higher or lower
	STATES eCurrState; //State for rendering as well as efficiency / other gameplay features

	int iFoodStored;
	int iWoodStored;

	float fEfficiency; //All states are to be multiplied by this float when used. 1.0f is normal efficiency

	virtual bool Handle(Message* msg);

	void DoFunction();

	//virtual void TheFunction(GameObject* go);

	Villager(GAMEOBJECT_TYPE typeValue = GO_NONE);
	~Villager();
};

#endif