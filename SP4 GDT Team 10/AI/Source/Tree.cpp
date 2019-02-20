#include "Tree.h"
#include "ConcreteMessages.h"

Tree::Tree(GAMEOBJECT_TYPE typeValue)
	: Environment(typeValue),
	iWoodAmount(10),
	eCurrState(FULL)
{
	//GameObject(typeValue);
	std::cout << "Tree Constructor" << std::endl;
}

Tree::~Tree()
{
}

bool Tree::Handle(Message* msg)
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