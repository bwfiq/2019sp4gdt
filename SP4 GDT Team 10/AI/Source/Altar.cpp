#include "Altar.h"
#include "ConcreteMessages.h"
#include "SceneData.h"

Altar::Altar(GAMEOBJECT_TYPE typeValue)
	: Building(typeValue),
	iFoodOffered(0)
{
	//GameObject(typeValue);
	std::cout << "Altar Constructor" << std::endl;
}

Altar::~Altar()
{
}

bool Altar::Handle(Message* msg)
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

void Altar::TheFunction(GameObject * go)
{
	std::cout << "Altar Function" << std::endl;
}

void Altar::Update(float dt)
{
	GameObject::Update(dt);
	//Spawning of disasters
}
