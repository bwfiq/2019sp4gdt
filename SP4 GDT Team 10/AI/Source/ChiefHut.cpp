#include "ChiefHut.h"
#include "ConcreteMessages.h"

ChiefHut::ChiefHut(GAMEOBJECT_TYPE typeValue) 
	: Building(typeValue),
	iHousingSpace(2)
{
	//GameObject(typeValue);
	std::cout << "ChiefHut Constructor" << std::endl;
}

ChiefHut::~ChiefHut()
{
}

bool ChiefHut::Handle(Message* msg)
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

void ChiefHut::TheFunction(GameObject * go)
{
	std::cout << "Chief Hut Function" << std::endl;
}
