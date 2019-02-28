#ifndef GAME_VILLAGER_H
#define GAME_VILLAGER_H

#include "GameObject.h"
#include "Mesh.h"

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
		MINING,
		STAT_TOTAL
	};

	enum STATES
	{
		HEALTHY = 0,
		
		TIRED,
		SICKLY,
		DYING,
		
		PANIC,
		STATES_TOTAL //Should not have any enums below this
	};

	float fStats[STAT_TOTAL]; //Stats for the efficiency of doing different activities, 1.0f is normal, can go higher or lower
	STATES eCurrState; //State for rendering as well as efficiency / other gameplay features

	int iFoodStored;
	int iWoodStored;
	int iStoneStored;

	int iMaxFoodStored;
	int iMaxWoodStored;
	int iMaxStoneStored;

	float fEfficiency; //All states are to be multiplied by this float when used. 1.0f is normal efficiency
	float fIdleTimer;

	float fActionTimer;

	float fRecalculate;

	float fEffectTimer_Panic;//a timer used to time the spawning of Panic effects (panic effect is EffectDirt btw)
	float fEffectTimer_Fight;//a timer used to time the spawning of Fight effects (fight effect is EffectCloud btw)

	virtual bool Handle(Message* msg);
	virtual void Update(float dt);

	void DoFunction();

	Mesh* mEquipment;

	//virtual void TheFunction(GameObject* go);

	Villager(GAMEOBJECT_TYPE typeValue = GO_NONE);
	~Villager();
};

#endif