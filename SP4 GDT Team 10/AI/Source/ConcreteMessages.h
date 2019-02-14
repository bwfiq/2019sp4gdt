#ifndef CONCRETE_MESSAGE_H
#define CONCRETE_MESSAGE_H

#include "Message.h"
#include "GameObject.h"

struct MessageWRU : public Message
{
	enum SEARCH_TYPE
	{
		SEARCH_NONE = 0,
		NEAREST_ENEMY,
		RANDOM_TARGET,
		PATH_TO_TARGET,
		PATH_TO_POINT,
		FIND_CHIEFHUT,
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
	};
	MessageObject(SEARCH_TYPE typeValue, float thresholdValue) :type(typeValue), threshold(thresholdValue) {}
	virtual ~MessageObject() {}

	GameObject *go;
	SEARCH_TYPE type;
	float threshold;
};
#endif
