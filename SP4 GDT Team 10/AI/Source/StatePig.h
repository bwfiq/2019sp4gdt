#ifndef STATE_PIG_H
#define STATE_PIG_H

#include "State.h"

/*
Pig States
---------------
Idle - Randomly walking around
Path - Moving
Eating - Eating from bush

Raised - Being Raised in village
*/

class StateEating : public State
{
public:
	StateEating(const std::string &stateID);
	virtual ~StateEating();
	

	//To be implemented by concrete states
	virtual void Enter(GameObject* m_go);
	virtual void Update(double dt, GameObject* m_go);
	virtual void Exit(GameObject* m_go);
};

#endif