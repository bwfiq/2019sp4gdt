#include "Altar.h"
#include "ConcreteMessages.h"
#include "SceneData.h"
#include "CalamityManager.h"
#include "CalamityTsunami.h"
#include "CalamityEarthquake.h"
#include "CalamityTornado.h"
#include "CalamityWorldEnd.h"

#include "PostOffice.h"

Altar::Altar(GAMEOBJECT_TYPE typeValue)
	: Building(typeValue),
	iFoodOffered(0),
	fMaxCountdown(20),
	fCountdown(20),
	fGodMad(100),
	bWorldEnded(false)
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
	if (bWorldEnded)
		return;
	CalamityManager* CM = CalamityManager::GetInstance();
	SceneData* SD = SceneData::GetInstance();
	//Spawning of disasters
	if (SD->GetReligionValue() <= 0)
	{
		fGodMad -= dt;
	}
	if (fGodMad <= 0.f)
	{
		SpawnCalamity(END_OF_THE_WORLD);
		bWorldEnded = true;
		return;
	}

	if (CM->GetCurrCalamity() == NULL)
	{
		float alpha = SD->GetReligionValue() / SD->GetMaxReligionValue();
		if (alpha < 0.3333333f)
		{
			//Furious Gods
			fCountdown -= dt * 2;
		}
		else if (alpha < 0.6666666f)
		{
			//Neutral Gods
			fCountdown -= dt * (SD->GetMaxReligionValue() / SD->GetReligionValue());
		}
		else
		{
			//Appeased Gods
			fCountdown -= dt * 0.5f;
		}

		if (fCountdown <= 0.f)
		{
			fCountdown = fMaxCountdown;
			SpawnCalamity((Calamity)(Math::RandIntMinMax(0, (int)(TOTAL)-1)));
		}
	}
}

void Altar::SpawnCalamity(Calamity calamity)
{
	CalamityManager* CM = CalamityManager::GetInstance();
	SceneData* SD = SceneData::GetInstance();

	switch (calamity)
	{
	case TSUNAMI:
		CM->AddToCalamityQueue(new CalamityTsunami());
		break;
	case EARTHQUAKE:
		CM->AddToCalamityQueue(new CalamityEarthquake());
		break;
	case TORNADO:
		CM->AddToCalamityQueue(new CalamityTornado());
		break;
	case END_OF_THE_WORLD:
		CM->AddToCalamityQueue(new CalamityWorldEnd());
		break;
	default:
		break;
	}
}
