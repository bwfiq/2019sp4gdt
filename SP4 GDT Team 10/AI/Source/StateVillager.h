#ifndef STATE_VILLAGER_H
#define STATE_VILLAGER_H

#include "State.h"

/*
Villager States
---------------
Idle
Path
Dosomething
Picked up
Dead
InHut
*/
class State
{
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