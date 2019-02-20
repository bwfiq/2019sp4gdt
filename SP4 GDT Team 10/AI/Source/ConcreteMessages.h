#ifndef CONCRETE_MESSAGE_H
#define CONCRETE_MESSAGE_H

#include "Message.h"
#include "GameObject.h"

class UIGameButton;

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
		FIND_NEAREST_GRANARY,
		FIND_NEAREST_WOODSHED,
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

struct MessageCameraShake : public Message
{
	enum SHAKE_TYPE
	{
		SHAKE_EARTHQUAKE,
	};
	MessageCameraShake(SHAKE_TYPE type, float intensity, float duration = 0.f) : type(type), intensity(intensity), duration(duration) {}
	virtual ~MessageCameraShake() {}
	SHAKE_TYPE type;
	float intensity;
	float duration;
};

struct MessageCalamityEarthquake : public Message
{

};

struct MessageDisplayDailyRequirement : public Message
{
	MessageDisplayDailyRequirement(UIGameButton* ui) : ui(ui) {}
	virtual ~MessageDisplayDailyRequirement() {}
	UIGameButton* ui;
};

struct MessageAltarOffer : public Message
{
	enum OFFER_TYPE {
		OFFER_FOOD,
	};
	MessageAltarOffer(OFFER_TYPE type) : type(type) {}
	virtual ~MessageAltarOffer() {}
	OFFER_TYPE type;
};
#endif
