#include "Bush.h"
#include "ConcreteMessages.h"

Bush::Bush(GAMEOBJECT_TYPE typeValue)
	: GameObject(typeValue)
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

void Bush::DoFunction()
{
	if(this->goTarget != NULL)
		this->goTarget->TheFunction(this);
}
