#include "Meteor.h"
#include "ConcreteMessages.h"


#include "Villager.h"
#include "Building.h"
#include "Logs.h"
#include "Environment.h"

#include "EffectManager.h"

Meteor::Meteor(GAMEOBJECT_TYPE typeValue)
	: GameObject(typeValue)
{
	//GameObject(typeValue);
	std::cout << "Meteor Constructor" << std::endl;

	fPower = 100.f;
	fEffectTimer_Fire = 0;
}

Meteor::~Meteor()
{
}

void Meteor::Update(float dt)
{
	GameObject::Update(dt);

	std::vector<std::vector<GameObject*>::iterator> removalList;
	for (std::vector<GameObject*>::iterator it = collidedObjects.begin(); it < collidedObjects.end(); ++it)
	{
		GameObject* go = (*it);
		if (!go->active)
		{
			removalList.push_back(it);
		}
	}
	while (!removalList.empty())
	{
		collidedObjects.erase(removalList.back());
		removalList.pop_back();
	}
}

void Meteor::Collided(GameObject * go)
{
	for (auto goCollided : collidedObjects)
	{
		if (goCollided == go)
		{
			//Collided once with the object
			return;
		}
	}
	std::cout << "Colliding Meteor" << std::endl;

	//Set go as collided with this Meteor once
	collidedObjects.push_back(go);

	Villager* goVil = dynamic_cast<Villager*>(go);
	if (goVil)
	{	
		//Chance to reduce the state of the GameObject
		goVil->active = false;
		EffectManager::GetInstance()->DoPrefabEffect(EffectManager::PREFAB_VILLAGER_DIE, goVil->pos);
		CSoundEngine::GetInstance()->PlayASound("death");

		//Reduce power of Meteor Wave
		//NA
		return;
	}
	Building* goBuilding = dynamic_cast<Building*>(go);
	if (goBuilding)
	{	
		//Chance to reduce the state of the GameObject
		bool bReduceState = false;
		float dmgReduction = 0;
		switch (goBuilding->eCurrTier)
		{
		case Building::STRAW:
			dmgReduction += 0;
			break;
		case Building::WOOD:
			dmgReduction += 10;
			break;
		case Building::STONE:
			dmgReduction += 25;
			break;
		case Building::FULL_STONE:
			dmgReduction += 50;
			break;
		}

		if (Math::RandFloatMinMax(0.f, 100.f) + dmgReduction < fPower)
			/*bReduceState = true;

		if (bReduceState)*/
		{
			switch (goBuilding->eCurrState)
			{
			case Building::COMPLETED:
				goBuilding->eCurrState = Building::BROKEN;
				break;
			case Building::BROKEN:
				goBuilding->active = false;
				break;
			default:
				break;
			}
		}

		//Reduce power of Meteor Wave
		if (go->type == GameObject::GO_LOGS)
			fPower -= 30.f;
		else
			fPower -= 10.f;

		return;
	}
	Environment* goEnvironment = dynamic_cast<Environment*>(go);
	if (goEnvironment)
	{	
		//Chance to reduce the state of the GameObject
		//NA

		//Reduce power of Meteor Wave
		switch (goEnvironment->type)
		{
		case GO_TREE:
			fPower -= 5.f;
			break;
		case GO_MOUNTAIN:
			fPower -= 10.f;
			break;
		}
		return;
	}
	
	//Some other type of GO
}

bool Meteor::Handle(Message* msg)
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

void Meteor::TheFunction(GameObject * go)
{
	std::cout << "Meteor Function" << std::endl;
}
