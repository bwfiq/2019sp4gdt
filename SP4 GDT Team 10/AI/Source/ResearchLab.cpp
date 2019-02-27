#include "ResearchLab.h"
#include "ConcreteMessages.h"

ResearchLab::ResearchLab(GAMEOBJECT_TYPE typeValue)
	: Building(typeValue)
{
	//GameObject(typeValue);
	std::cout << "ResearchLab Constructor" << std::endl;
	buildingName = "Research Lab";
}

ResearchLab::~ResearchLab()
{
}

bool ResearchLab::Handle(Message* msg)
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

void ResearchLab::TheFunction(GameObject * go)
{
	std::cout << "Research Lab Function" << std::endl;
}
