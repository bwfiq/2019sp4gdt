#include "Villager.h"
#include "ConcreteMessages.h"

Villager::Villager(GAMEOBJECT_TYPE typeValue)
	: GameObject(typeValue),
	fEfficiency(1.f),
	eCurrState(HEALTHY),
	iFoodStored(0),
	iWoodStored(0),
	iStoneStored(0),
	iMaxFoodStored(5),
	iMaxWoodStored(5),
	iMaxStoneStored(5),
	fIdleTimer(3),
	mEquipment(NULL),
	fRecalculate(1.5f),
	fEffectTimer_Panic(0),
	fEffectTimer_Fight(0)
{
	//GameObject(typeValue);
	for (int i = 0; i < STAT_TOTAL; ++i)
	{
		fStats[i] = 1.f;
	}
	std::cout << "Villager Constructor" << std::endl;
}

Villager::~Villager()
{
}

bool Villager::Handle(Message* msg)
{
	MessageObject* messageObject = dynamic_cast<MessageObject*>(msg);
	switch (messageObject->type)
	{
	case MessageObject::ACTIVE_OBJECT:
		if (this->active)
		{
			delete msg;
			return true;
		}
		break;
	}
	delete msg;
	return false;
}

void Villager::Update(float dt)
{
	GameObject::Update(dt);
	//minAABB.y *= 2.222f;
	maxAABB.y *= 4.444f;
}


void Villager::DoFunction()
{
	if(this->goTarget != NULL)
		this->goTarget->TheFunction(this);
}
