#ifndef GAME_VILLAGER_H
#define GAME_VILLAGER_H

#include "GameObject.h"

struct Villager : public GameObject
{
	/*
	bool moveLeft;
	bool moveRight;
	bool moveUp;
	bool moveDown;*/



	virtual bool Handle(Message* msg);

	Villager(GAMEOBJECT_TYPE typeValue = GO_NONE);
	~Villager();
};

#endif