#include "StateVillager.h"

#include "SMManager.h"
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
	if (m_go->goTarget != NULL)
	{
		m_go->m_nextState = SMManager::GetInstance()->GetSM(m_go->smID)->GetState("Path");
		return;
	}
	//m_go->pos += Vector3(-1, 0, 0) * dt * MOVE_SPEED;
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
	//Temporary Movement
	if ((m_go->pos - m_go->goTarget->pos).LengthSquared() < (m_go->scale.x + m_go->goTarget->scale.x) * (m_go->scale.x + m_go->goTarget->scale.x) * 0.5f)
	{
		m_go->goTarget = NULL;
		m_go->m_nextState = SMManager::GetInstance()->GetSM(m_go->smID)->GetState("Idle");
		return;
	}
	m_go->pos += (m_go->goTarget->pos - m_go->pos).Normalized() * dt * MOVE_SPEED;
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