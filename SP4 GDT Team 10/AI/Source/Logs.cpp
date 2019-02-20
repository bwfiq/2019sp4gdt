#include "Logs.h"
#include "ConcreteMessages.h"

Logs::Logs(GAMEOBJECT_TYPE typeValue)
	: Building(typeValue)
{
	//GameObject(typeValue);
	std::cout << "Logs Constructor" << std::endl;
}

Logs::~Logs()
{
}

bool Logs::Handle(Message* msg)
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

void Logs::TheFunction(GameObject * go)
{
	std::cout << "Logs Function" << std::endl;
}
