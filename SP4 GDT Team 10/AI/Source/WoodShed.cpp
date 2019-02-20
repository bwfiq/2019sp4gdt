#include "WoodShed.h"
#include "ConcreteMessages.h"

WoodShed::WoodShed(GAMEOBJECT_TYPE typeValue)
	: Building(typeValue)
	, woodCapacity(20)
{
	//GameObject(typeValue);
	std::cout << "WoodShed Constructor" << std::endl;
}

WoodShed::~WoodShed()
{
}

bool WoodShed::Handle(Message* msg)
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

void WoodShed::TheFunction(GameObject * go)
{
	std::cout << "WoodShed Function" << std::endl;
}
