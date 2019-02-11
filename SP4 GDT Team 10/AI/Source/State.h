#ifndef STATE_H
#define STATE_H

#include <string>

struct GameObject;

class State
{
	const std::string m_stateID;
protected:
	State(const std::string &stateID);
public:
	virtual ~State();
	const std::string& GetStateID();

	//To be implemented by concrete states
	virtual void Enter(GameObject*) = 0;
	virtual void Update(double dt, GameObject*) = 0;
	virtual void Exit(GameObject*) = 0;
};

#endif