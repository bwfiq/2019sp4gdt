#ifndef CONCRETE_MESSAGE_H
#define CONCRETE_MESSAGE_H

#include "Message.h"
#include "GameObject.h"
#include <vector>

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
		FIND_NEAREST_LUSH_BUSH,
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

struct MessageWarnLane : public Message
{
	MessageWarnLane() {}
	virtual ~MessageWarnLane() {}
	std::vector<unsigned> lanes;
};

struct MessageCalamityEarthquake : public Message
{
	enum EARTHQUAKE_TYPE
	{
		INTENSE,
		STOPPING
	};
	MessageCalamityEarthquake(float fPower, EARTHQUAKE_TYPE type) : fPower(fPower), type(type) {}
	virtual ~MessageCalamityEarthquake(){}
	float fPower;
	EARTHQUAKE_TYPE type;
};

struct MessageCalamityTsunami : public Message
{
	MessageCalamityTsunami() {}
	virtual ~MessageCalamityTsunami() {}
	std::vector<unsigned> lanes;
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

struct MessageMoveButton : public Message
{
	MessageMoveButton() {}
	virtual ~MessageMoveButton() {}
};
struct MessageBuildBuildings : public Message
{
	MessageBuildBuildings(GameObject::GAMEOBJECT_TYPE type) : type(type) {}
	virtual ~MessageBuildBuildings() {}
	GameObject::GAMEOBJECT_TYPE type;
};
struct MessageCreateBuildUIs : public Message
{
	MessageCreateBuildUIs() {}
	virtual ~MessageCreateBuildUIs() {}
};
#endif
