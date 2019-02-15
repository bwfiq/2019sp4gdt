#include "Altar.h"
#include "ConcreteMessages.h"

Altar::Altar(GAMEOBJECT_TYPE typeValue)
	: Building(typeValue)
{
	//GameObject(typeValue);
	std::cout << "ChiefHut Constructor" << std::endl;
}

Altar::~Altar()
{
}

bool Altar::Handle(Message* msg)
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

void Altar::TheFunction(GameObject * go)
{
	std::cout << "Altar Function" << std::endl;
}
