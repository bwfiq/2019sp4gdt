#ifndef SMMANAGER_H
#define SMMANAGER_H

#include "SingletonTemplate.h"
#include "StateMachine.h"

class SMManager : public Singleton<SMManager>
{
public:
	std::map<std::string, StateMachine*> m_smMap;

	SMManager();
	~SMManager();

	void AddSM(StateMachine* sm);
	StateMachine* GetSM(const std::string& smID);
};
#endif