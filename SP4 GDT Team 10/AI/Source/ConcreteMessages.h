#ifndef CONCRETE_MESSAGE_H
#define CONCRETE_MESSAGE_H

#include "Message.h"
#include "GameObject.h"
#include <vector>

class UIGameButton;
struct GridPt;

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
		FIND_NEAREST_STONESHED,
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
		SHAKE_METEOR,
		SHAKE_DESTRUCTION,
	};
	MessageCameraShake(SHAKE_TYPE type, float intensity, float duration = 0.f) : type(type), intensity(intensity), duration(duration) {}
	virtual ~MessageCameraShake() {}
	SHAKE_TYPE type;
	float intensity;
	float duration;
};

struct MessageCalamityEnd : public Message
{
	MessageCalamityEnd(){}
	virtual ~MessageCalamityEnd() {}
};
struct MessageWarnLane : public Message
{
	MessageWarnLane() {}
	virtual ~MessageWarnLane() {}
	std::vector<unsigned> lanes;
};

struct MessageWarnGrids : public Message
{
	MessageWarnGrids(std::vector<GridPt> grids) : grids(grids) {}
	virtual ~MessageWarnGrids() {}
	std::vector<GridPt> grids;
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

struct MessageCalamityBlizzard : public Message
{
	enum BLIZZARD_TYPE
	{
		INTENSE,
		STOPPING
	};
	MessageCalamityBlizzard(float fPower, BLIZZARD_TYPE type) : fPower(fPower), type(type) {}
	virtual ~MessageCalamityBlizzard() {}
	float fPower;
	BLIZZARD_TYPE type;
};

struct MessageCalamityMeteorShower : public Message
{
	MessageCalamityMeteorShower(std::vector<GridPt> meteorSpawnGrids) : meteorSpawnGrids(meteorSpawnGrids) {}
	virtual ~MessageCalamityMeteorShower() {}
	std::vector<GridPt> meteorSpawnGrids;
};

struct MessageCalamityTsunami : public Message
{
	MessageCalamityTsunami() {}
	virtual ~MessageCalamityTsunami() {}
	std::vector<unsigned> lanes;
};

struct MessageCalamityWorldEnd : public Message
{
	MessageCalamityWorldEnd() {}
	virtual ~MessageCalamityWorldEnd() {}
};

struct MessageCalamityTornado : public Message
{
	MessageCalamityTornado(float fPower) : fPower(fPower) {}
	virtual ~MessageCalamityTornado() {}
	float fPower;
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

struct MessageResearch : public Message
{
	MessageResearch() {}
	virtual ~MessageResearch() {}
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