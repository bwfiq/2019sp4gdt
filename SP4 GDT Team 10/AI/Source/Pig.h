#ifndef GAME_PIG_H
#define GAME_PIG_H

#include "GameObject.h"
#include "Mesh.h"

struct Pig : public GameObject
{
	/*
	bool moveLeft;
	bool moveRight;
	bool moveUp;
	bool moveDown;*/
	enum State
	{
		WILD,
		TAME,
		TOTAL,
	};
	enum Movement
	{
		WALKING,
		ROLLING,
	};

	float fActionTimer;
	float fIdleTimer;
	float fEnergy;
	float fTimer;
	int iFoodAmount;

	State state;
	Movement movement;

	virtual bool Handle(Message* msg);
	//virtual void TheFunction(GameObject* go);

	Pig(GAMEOBJECT_TYPE typeValue = GO_NONE);
	~Pig();
};

#endif