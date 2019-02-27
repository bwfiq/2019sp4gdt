#include "Pig.h"
#include "ConcreteMessages.h"

Pig::Pig(GAMEOBJECT_TYPE typeValue)
	: GameObject(typeValue),
	state(WILD),
	fEnergy(50),
	fIdleTimer(3),
	iFoodAmount(10), 
	fTimer(1),
	movement(WALKING)
{
	std::cout << "Pig Constructor" << std::endl;
}

Pig::~Pig()
{
}

bool Pig::Handle(Message* msg)
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