#include "StateVillager.h"

#include "PostOffice.h"
#include "ConcreteMessages.h"
#include "SceneData.h"
#include "SMManager.h"
#include "ProjectileManager.h"
#include <iostream>
//State::State(const std::string & stateID)
//	: m_stateID(stateID)
//{
//}
//
//State::~State()
//{
//}
//
//const std::string & State::GetStateID()
//{
//	return m_stateID;
//}

static float MOVE_SPEED = 5.f;

//StateIdle
StateIdle::StateIdle(const std::string & stateID)
	: State(stateID)
{
}

StateIdle::~StateIdle()
{
}


void StateIdle::Enter(GameObject* m_go)
{
	std::cout << "Enter Idle State" << std::endl;
}

void StateIdle::Update(double dt, GameObject* m_go)
{
	if (m_go->goTarget != NULL || m_go->target != NULL)
	{
		m_go->m_nextState = SMManager::GetInstance()->GetSM(m_go->smID)->GetState("Path");
		if (m_go->goTarget != NULL)
		{
			MessageWRU* messagewru = new MessageWRU(m_go, MessageWRU::PATH_TO_TARGET, 1);
			PostOffice::GetInstance()->Send("Scene", messagewru);
		}
		return;
	}
}

void StateIdle::Exit(GameObject* m_go)
{
}

//StatePath
StatePath::StatePath(const std::string & stateID)
	: State(stateID)
{
}

StatePath::~StatePath()
{
}

void StatePath::Enter(GameObject * m_go)
{
	std::cout << "Enter Path State" << std::endl;
}

void StatePath::Update(double dt, GameObject * m_go)
{
	SceneData* SD = SceneData::GetInstance();
	if (m_go->target != NULL || m_go->goTarget != NULL)
	{
		
		if (m_go->target != NULL)
		{
			if ((m_go->pos - m_go->target).LengthSquared() < 0.05f)
			{
				m_go->pos = m_go->target;
				m_go->target = NULL;
				m_go->m_nextState = SMManager::GetInstance()->GetSM(m_go->smID)->GetState("Idle");
				return;
			}
			m_go->pos += (m_go->target - m_go->pos).Normalized() * dt * MOVE_SPEED;
		}
		else if (m_go->goTarget != NULL)
		{
			if (!m_go->path.empty())
			{
				Vector3 ptPos = GetGridPos(m_go->path.back()); //Position of gridPt in world space
				Vector3 direction = ptPos - m_go->pos;
				direction.y = 0;
				if (direction.LengthSquared() < 0.05f)
				{
					m_go->pos += direction;
					m_go->path.pop_back();
				}
				else
					m_go->pos += (direction).Normalized() * dt * MOVE_SPEED;
			}
			else
			{
				m_go->goTarget = NULL;
				m_go->m_nextState = SMManager::GetInstance()->GetSM(m_go->smID)->GetState("Idle");
				return;
			}
		}
	}
	else
	{
		m_go->m_nextState = SMManager::GetInstance()->GetSM(m_go->smID)->GetState("Idle");
		while (!m_go->path.empty())
		{
			m_go->path.pop_back();
		}
	}
}

void StatePath::Exit(GameObject * m_go)
{
}

//StateChopTree
StateChopTree::StateChopTree(const std::string & stateID)
	: State(stateID)
{
}

StateChopTree::~StateChopTree()
{
}

void StateChopTree::Enter(GameObject * m_go)
{
	std::cout << "Enter Chop Tree State" << std::endl;
}

void StateChopTree::Update(double dt, GameObject * m_go)
{
}

void StateChopTree::Exit(GameObject * m_go)
{
}

//StateForaging
StateForaging::StateForaging(const std::string & stateID)
	: State(stateID)
{
}

StateForaging::~StateForaging()
{
}

void StateForaging::Enter(GameObject * m_go)
{
	std::cout << "Enter Foraging State" << std::endl;
}

void StateForaging::Update(double dt, GameObject * m_go)
{
}

void StateForaging::Exit(GameObject * m_go)
{
}

//StateAttack
StateAttack::StateAttack(const std::string & stateID)
	: State(stateID)
{
}

StateAttack::~StateAttack()
{
}

void StateAttack::Enter(GameObject* m_go)
{
	std::cout << "Enter Attack State" << std::endl;
}

void StateAttack::Update(double dt, GameObject* m_go)
{
}

void StateAttack::Exit(GameObject* m_go)
{
}

//StatePickedUp
StatePickedUp::StatePickedUp(const std::string & stateID)
	: State(stateID)
{
}

StatePickedUp::~StatePickedUp()
{
}

void StatePickedUp::Enter(GameObject* m_go)
{
	std::cout << "Enter PickedUp State" << std::endl;
}

void StatePickedUp::Update(double dt, GameObject* m_go)
{
}

void StatePickedUp::Exit(GameObject* m_go)
{
}

//StateDead
StateDead::StateDead(const std::string & stateID)
	: State(stateID)
{
}

StateDead::~StateDead()
{
}

void StateDead::Enter(GameObject* m_go)
{
	std::cout << "Enter Dead State" << std::endl;
}

void StateDead::Update(double dt, GameObject* m_go)
{
}

void StateDead::Exit(GameObject* m_go)
{
}

//StateInHut
StateInHut::StateInHut(const std::string & stateID)
	: State(stateID)
{
}

StateInHut::~StateInHut()
{
}


void StateInHut::Enter(GameObject* m_go)
{
	std::cout << "Enter InHut State" << std::endl;
}

void StateInHut::Update(double dt, GameObject* m_go)
{
}

void StateInHut::Exit(GameObject* m_go)
{
}