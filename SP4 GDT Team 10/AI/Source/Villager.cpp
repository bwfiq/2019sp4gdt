#include "Villager.h"
#include "ConcreteMessages.h"

Villager::Villager(GAMEOBJECT_TYPE typeValue) : GameObject(typeValue)
{
	//GameObject(typeValue);
	this->type;
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