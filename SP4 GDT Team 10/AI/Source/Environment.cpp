#include "Environment.h"
#include "ConcreteMessages.h"

Environment::Environment(GAMEOBJECT_TYPE typeValue)
	: GameObject(typeValue)
	,fTimer(1)
{
	//GameObject(typeValue);
	std::cout << "Environment Constructor" << std::endl;
}

Environment::~Environment()
{
}

bool Environment::Handle(Message* msg)
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