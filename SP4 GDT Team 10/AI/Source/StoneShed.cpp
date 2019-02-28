#include "StoneShed.h"
#include "ConcreteMessages.h"

StoneShed::StoneShed(GAMEOBJECT_TYPE typeValue)
	: Building(typeValue)
	, stoneCapacity(20)
{
	//GameObject(typeValue);
	std::cout << "StoneShed Constructor" << std::endl;
	buildingName = "StoneShed";
}

StoneShed::~StoneShed()
{
}

bool StoneShed::Handle(Message* msg)
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

void StoneShed::TheFunction(GameObject * go)
{
	std::cout << "StoneShed Function" << std::endl;
}
