#ifndef STATE_VILLAGER_H
#define STATE_VILLAGER_H

#include "State.h"

/*
Villager States
---------------
Idle
Path
Dosomething
ChopTree
Foraging
Attack

Picked up
Dead
InHut
*/
class StateIdle : public State
{
public:
	StateIdle(const std::string &stateID);
	virtual ~StateIdle();
	

	//To be implemented by concrete states
	virtual void Enter(GameObject* m_go);
	virtual void Update(double dt, GameObject* m_go);
	virtual void Exit(GameObject* m_go);
};

class StatePath : public State
{
public:
	StatePath(const std::string &stateID);
	virtual ~StatePath();
	

	//To be implemented by concrete states
	virtual void Enter(GameObject* m_go);
	virtual void Update(double dt, GameObject* m_go);
	virtual void Exit(GameObject* m_go);
};

/*
class StateDoSomething : public State
{
protected:
	StateIdle(const std::string &stateID);
public:
	virtual ~StateIdle();
	

	//To be implemented by concrete states
	virtual void Enter(GameObject* m_go);
	virtual void Update(double dt, GameObject* m_go);
	virtual void Exit(GameObject* m_go);
};*/

class StateChopTree : public State
{
public:StateChopTree(const std::string &stateID);
	virtual ~StateChopTree();
	

	//To be implemented by concrete states
	virtual void Enter(GameObject* m_go);
	virtual void Update(double dt, GameObject* m_go);
	virtual void Exit(GameObject* m_go);
};

class StateForaging : public State
{
public:
	StateForaging(const std::string &stateID);
	virtual ~StateForaging();
	

	//To be implemented by concrete states
	virtual void Enter(GameObject* m_go);
	virtual void Update(double dt, GameObject* m_go);
	virtual void Exit(GameObject* m_go);
};

class StateAttack : public State
{
public:
	StateAttack(const std::string &stateID);
	virtual ~StateAttack();
	

	//To be implemented by concrete states
	virtual void Enter(GameObject* m_go);
	virtual void Update(double dt, GameObject* m_go);
	virtual void Exit(GameObject* m_go);
};

class StatePickedUp : public State
{
public:	
	StatePickedUp(const std::string &stateID);
	virtual ~StatePickedUp();
	

	//To be implemented by concrete states
	virtual void Enter(GameObject* m_go);
	virtual void Update(double dt, GameObject* m_go);
	virtual void Exit(GameObject* m_go);
};

class StateDead : public State
{
public:
	StateDead(const std::string &stateID);
	virtual ~StateDead();

	//To be implemented by concrete states
	virtual void Enter(GameObject* m_go);
	virtual void Update(double dt, GameObject* m_go);
	virtual void Exit(GameObject* m_go);
};

class StateInHut : public State
{
public:
	StateInHut(const std::string &stateID);
	virtual ~StateInHut();
	

	//To be implemented by concrete states
	virtual void Enter(GameObject* m_go);
	virtual void Update(double dt, GameObject* m_go);
	virtual void Exit(GameObject* m_go);
};

#endif