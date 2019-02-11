#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include <string>
#include <map>
#include "State.h"

#include "GameObject.h"

class StateMachine
{
	const std::string m_smID;
	std::map<std::string, State*> m_stateMap;
public:
	StateMachine();
	StateMachine(const std::string &smID);
	~StateMachine();

	const std::string& GetSMID();
	State* GetState(const std::string &stateID);
	void AddState(State *newState);
	void SetNextState(const std::string &nextStateID, GameObject* m_go);
	const std::string& GetCurrentState(GameObject* m_go);
	void Update(double dt, GameObject* m_go);
};

#endif