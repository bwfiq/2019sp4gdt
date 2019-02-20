#include "House.h"
#include "ConcreteMessages.h"

House::House(GAMEOBJECT_TYPE typeValue)
	: Building(typeValue)
	, iHousingSpace(2)
{
	//GameObject(typeValue);
	std::cout << "House Constructor" << std::endl;
}

House::~House()
{
}

bool House::Handle(Message* msg)
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

void House::TheFunction(GameObject * go)
{
	std::cout << "House Function" << std::endl;
}
