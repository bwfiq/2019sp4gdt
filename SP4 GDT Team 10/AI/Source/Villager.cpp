#include "Villager.h"
#include "ConcreteMessages.h"

Villager::Villager(GAMEOBJECT_TYPE typeValue)
	: GameObject(typeValue),
	fEfficiency(1.f),
	eCurrState(HEALTHY)
{
	//GameObject(typeValue);
	for (int i = 0; i < STAT_TOTAL; ++i)
	{
		iStats[i] = 100;
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

void Villager::DoFunction()
{
	if(this->goTarget != NULL)
		this->goTarget->TheFunction(this);
}
