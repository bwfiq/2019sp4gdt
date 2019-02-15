#include "Building.h"
#include "ConcreteMessages.h"

Building::Building(GAMEOBJECT_TYPE typeValue)
	: GameObject(typeValue),
	eCurrState(COMPLETED),
	bBuilt(false)
{
	//GameObject(typeValue);
	std::cout << "Building Constructor" << std::endl;
}

Building::~Building()
{
}

bool Building::Handle(Message* msg)
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

void Building::TheFunction(GameObject* go)
{
	std::cout << "Building Function" << std::endl;
}