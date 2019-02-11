#include "StateMachine.h"

StateMachine::StateMachine()
	:
	m_smID("")
{
}

StateMachine::StateMachine(const std::string & smID) 
	:
	m_smID(smID)
{
}

StateMachine::~StateMachine()
{
	for (std::map<std::string, State*>::iterator it = m_stateMap.begin(); it != m_stateMap.end(); ++it)
	{
		delete it->second;
	}
	m_stateMap.clear();
}

const std::string & StateMachine::GetSMID()
{
	return m_smID;
}

State * StateMachine::GetState(const std::string & stateID)
{
	if (m_stateMap.find(stateID) == m_stateMap.end())
		return NULL;
	else
		return m_stateMap.find(stateID)->second;
}

void StateMachine::AddState(State *newState)
{
	if (!newState)
		return;
	if (m_stateMap.find(newState->GetStateID()) != m_stateMap.end())
		return;
	m_stateMap.insert(std::pair<std::string, State*>(newState->GetStateID(), newState));
}

void StateMachine::SetNextState(const std::string &nextStateID, GameObject* m_go)
{
	std::map<std::string, State*>::iterator it = m_stateMap.find(nextStateID);
	if (it != m_stateMap.end())
	{
		m_go->m_nextState = (State *)it->second;
	}
}

const std::string & StateMachine::GetCurrentState(GameObject* m_go)
{
	if (m_go->m_currState)
		return m_go->m_currState->GetStateID();
	return "<No states>";
}

void StateMachine::Update(double dt, GameObject* m_go)
{
	if (m_go->m_nextState != m_go->m_currState)
	{
		(m_go->m_currState)->Exit(m_go);
		m_go->m_currState = m_go->m_nextState;
		(m_go->m_currState)->Enter(m_go);
	}
	(m_go->m_currState)->Update(dt, m_go);
}
