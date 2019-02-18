#include "Mountain.h"
#include "ConcreteMessages.h"

Mountain::Mountain(GAMEOBJECT_TYPE typeValue)
	: Environment(typeValue),
	iStoneAmount(10),
	iStoneGain(5)
{
	//GameObject(typeValue);
	std::cout << "Mountain Constructor" << std::endl;
}

Mountain::~Mountain()
{
}

bool Mountain::Handle(Message* msg)
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