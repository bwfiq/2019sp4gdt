#include "Tsunami.h"
#include "ConcreteMessages.h"

Tsunami::Tsunami(GAMEOBJECT_TYPE typeValue)
	: GameObject(typeValue)
{
	//GameObject(typeValue);
	std::cout << "Tsunami Constructor" << std::endl;
}

Tsunami::~Tsunami()
{
}

bool Tsunami::Handle(Message* msg)
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

void Tsunami::TheFunction(GameObject * go)
{
	std::cout << "Tsunami Function" << std::endl;
}
