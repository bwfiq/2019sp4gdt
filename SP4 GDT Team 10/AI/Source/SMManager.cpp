#include "SMManager.h"

#include "StateVillager.h"
#include "StatePig.h"

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
	
	else if (smID == "VillagerSM")
	{
		StateMachine* SMVillager = new StateMachine("VillagerSM");
		SMVillager->AddState(new StateIdle("Idle"));
		SMVillager->AddState(new StateAttack("Attack"));
		SMVillager->AddState(new StatePath("Path"));
		SMVillager->AddState(new StateDead("Dead"));
		SMVillager->AddState(new StateChopTree("ChopTree"));
		SMVillager->AddState(new StateForaging("Foraging"));
		SMVillager->AddState(new StatePickedUp("PickedUp"));
		SMVillager->AddState(new StateInHut("InHut"));
		SMVillager->AddState(new StateConstructing("Constructing"));
		SMVillager->AddState(new StateMining("Mining"));
		SMVillager->AddState(new StateHunting("Hunting"));
		this->AddSM(SMVillager);
		return SMVillager;
	}
	else if (smID == "PigSM")
	{
		StateMachine* SMPig = new StateMachine("PigSM");
		SMPig->AddState(new StateIdle("Idle"));
		SMPig->AddState(new StatePath("Path"));
		SMPig->AddState(new StateEating("Eating"));
		SMPig->AddState(new StateDying("Dying"));
		this->AddSM(SMPig);
		return SMPig;
	}
	else
	{
		return NULL;
	}
}