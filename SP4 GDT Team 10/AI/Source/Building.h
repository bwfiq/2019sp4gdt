#ifndef GAME_BUILDING_H
#define GAME_BUILDING_H

#include "GameObject.h"

struct Building : public GameObject
{
	/*
	bool moveLeft;
	bool moveRight;
	bool moveUp;
	bool moveDown;*/



	virtual bool Handle(Message* msg);

	Building(GAMEOBJECT_TYPE typeValue = GO_NONE);
	~Building();
};

#endif