#include "SMManager.h"

SMManager::SMManager()
{

}

SMManager::~SMManager()
{
	for (std::map<std::string, StateMachine*>::iterator it = m_smMap.begin(); it != m_smMap.end(); ++it)
	{
		delete it->second;
	}
	m_smMap.clear();
}

void SMManager::AddSM(StateMachine * sm)
{
	if (m_smMap.find(sm->GetSMID()) != m_smMap.end())
		return;
	m_smMap.insert(std::pair<std::string, StateMachine*>(sm->GetSMID(), sm));
}

StateMachine* SMManager::GetSM(const std::string& smID)
{
	if (m_smMap.find(smID) != m_smMap.end())
	{
		return m_smMap.find(smID)->second;
	}
	/*
	else if (smID == "MinionSM")
	{
		StateMachine* SMMinion = new StateMachine("MinionSM");
		SMMinion->AddState(new StateChase("Chase"));
		SMMinion->AddState(new StateAttack("Attack"));
		SMMinion->AddState(new StatePath("Path"));
		SMMinion->AddState(new StateDead("Dead"));
		this->AddSM(SMMinion);
		return SMMinion;
	}*/
	else
	{
		return NULL;
	}
}