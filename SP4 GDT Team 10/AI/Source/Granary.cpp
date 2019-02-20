#include "Granary.h"
#include "ConcreteMessages.h"

Granary::Granary(GAMEOBJECT_TYPE typeValue)
	: Building(typeValue)
	, foodCapacity(20)
{
	//GameObject(typeValue);
	std::cout << "Granary Constructor" << std::endl;
}

Granary::~Granary()
{
}

bool Granary::Handle(Message* msg)
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

void Granary::TheFunction(GameObject * go)
{
	std::cout << "Granary Function" << std::endl;
}
