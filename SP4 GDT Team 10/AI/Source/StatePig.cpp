#include "Pig.h"
#include "StatePig.h"

#include "PostOffice.h"
#include "ConcreteMessages.h"
#include "SceneData.h"
#include "SMManager.h"
#include "ProjectileManager.h"
#include <iostream>

#include "EffectManager.h"

#include "Villager.h"
#include "Building.h"
#include "Bush.h"
#include "Tree.h"
#include "Mountain.h"

#include "AnimationWalk.h"
#include "AnimationPanic.h"
#include "AnimationTiredWalk.h"
#include "AnimationPickUp.h"
#include "AnimationChopping.h"
#include "AnimationForage.h"
#include "AnimationConstructing.h"

#include "MousePicker.h"
#include "MouseController.h"
//StateEating
StateEating::StateEating(const std::string & stateID)
	: State(stateID)
{
}

StateEating::~StateEating()
{
}

void StateEating::Enter(GameObject * m_go)
{
	std::cout << "Enter Eating State" << std::endl;
	Pig* goPig = static_cast<Pig*>(m_go);
	goPig->fActionTimer = static_cast<Environment*>(m_go->goTarget)->fTimer;
	m_go->scale *= 0.2f;
	m_go->pos.x += m_go->goTarget->scale.x * 0.3f;
	m_go->pos.y = m_go->scale.y * 0.5f;
	m_go->pos.z += m_go->goTarget->scale.z * 0.3f;
	m_go->direction.Set(-1, 0, -1);
	m_go->direction.Normalize();
	CSoundEngine::GetInstance()->PlayASound("munching", true);

	//SceneData* SD = SceneData::GetInstance();
	//goVil->mEquipment = SD->GetMesh("basket");
	//goVil->GiveAnimation(new AnimationForage());
}

void StateEating::Update(double dt, GameObject * m_go)
{

	if (m_go->goTarget == NULL || !m_go->goTarget->active)
	{
		m_go->goTarget = NULL;
		m_go->m_nextState = SMManager::GetInstance()->GetSM(m_go->smID)->GetState("Idle");
		return;
	}
	//In StateForaging, the goTarget must be a Bush class
	if (m_go->goTarget->type != GameObject::GO_BUSH)
	{
		std::cout << "Wrong State : Eating" << std::endl;
		return;
	}

	Bush* bushGo = static_cast<Bush*>(m_go->goTarget);
	Pig* goPig = static_cast<Pig*>(m_go);
	if (bushGo->eCurrState == Bush::LUSH)
	{
		if (goPig->fActionTimer <= 0.f)
		{
			//Insert gathering time here
			goPig->fEnergy += 40;
			goPig->iFoodAmount += bushGo->iFoodAmount;
			bushGo->eCurrState = Bush::DEPLETED;
			m_go->goTarget = NULL;
			m_go->m_nextState = SMManager::GetInstance()->GetSM(m_go->smID)->GetState("Idle");
			return;
		}
		else
		{
			goPig->fActionTimer -= dt;
		}
	}
	else
	{
		m_go->goTarget = NULL;
		m_go->m_nextState = SMManager::GetInstance()->GetSM(m_go->smID)->GetState("Idle");
	}
}

void StateEating::Exit(GameObject * m_go)
{
	Pig* goPig = static_cast<Pig*>(m_go);
	goPig->fActionTimer = 0;

	m_go->scale *= 5;
	m_go->pos.y = m_go->scale.y * 0.5f;
	m_go->ClearAnimation();
	CSoundEngine::GetInstance()->StopASound("munching");
	//goVil->mEquipment = NULL;
}

//StateDying
StateDying::StateDying(const std::string & stateID)
	: State(stateID)
{
}

StateDying::~StateDying()
{
}

void StateDying::Enter(GameObject * m_go)
{
	std::cout << "Enter Dying State" << std::endl;
	std::cout << "Clearing existing m_go path" << std::endl;

	while (!m_go->path.empty())
	{
		m_go->path.pop_back();
	}
}

void StateDying::Update(double dt, GameObject * m_go)
{
	Pig* goPig = dynamic_cast<Pig*>(m_go);
	if (goPig)
	{
		goPig->fEnergy = 0;
		goPig->active = false;
		EffectManager::GetInstance()->DoPrefabEffect(EffectManager::PREFAB_VILLAGER_DIE, goPig->pos);
		CSoundEngine::GetInstance()->PlayASound("death");
	}
}

void StateDying::Exit(GameObject * m_go)
{
}