#ifndef GAME_METEOROBJECT_H
#define GAME_METEOROBJECT_H

#include "GameObject.h"

struct Meteor : public GameObject
{
	virtual bool Handle(Message* msg);
	virtual void Update(float dt);
	virtual void TheFunction(GameObject* go);

	Meteor(GAMEOBJECT_TYPE typeValue = GO_NONE);
	~Meteor();

	float fPower; //Strength of the wave
	float fEffectTimer_Fire;

	void Collided(GameObject* go);

	std::vector<GameObject*> collidedObjects;
};

#endif