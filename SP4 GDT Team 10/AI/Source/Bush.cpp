#include "Bush.h"
#include "ConcreteMessages.h"

Bush::Bush(GAMEOBJECT_TYPE typeValue)
	: Environment(typeValue),
	iFoodAmount(10),
	eCurrState(LUSH)
{
	//GameObject(typeValue);
	std::cout << "Bush Constructor" << std::endl;
}

Bush::~Bush()
{
}

bool Bush::Handle(Message* msg)
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