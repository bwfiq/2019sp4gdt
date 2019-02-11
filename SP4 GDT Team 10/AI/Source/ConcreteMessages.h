#ifndef CONCRETE_MESSAGE_H
#define CONCRETE_MESSAGE_H

#include "Message.h"
#include "GameObject.h"

struct MessageWRU : public Message
{
	enum SEARCH_TYPE
	{
		SEARCH_NONE = 0,
		NEAREST_SHARK,
		NEAREST_FISHFOOD,
		NEAREST_FULLFISH,
		HIGHEST_ENERGYFISH,
		NEAREST_WATER,
		FOOD,
		WHAT_TO_DO,
		ANY_NODE,
		MY_BASE,
		NEAREST_ENEMY,
	};
	MessageWRU(GameObject *goValue, SEARCH_TYPE typeValue, float thresholdValue) : go(goValue), type(typeValue), threshold(thresholdValue) {}
	virtual ~MessageWRU() {}

	GameObject *go;
	SEARCH_TYPE type;
	float threshold;
};

struct MessageObject : public Message
{
	enum SEARCH_TYPE
	{
		SEARCH_NONE = 0,
		ACTIVE_OBJECT,
		ACTIVE_FISH,
		KILL_FISH,
		KILL_SHARK,
		KILL_FISHFOOD,
	};
	MessageObject(SEARCH_TYPE typeValue, float thresholdValue) :type(typeValue), threshold(thresholdValue) {}
	virtual ~MessageObject() {}

	GameObject *go;
	SEARCH_TYPE type;
	float threshold;
};
#endif
