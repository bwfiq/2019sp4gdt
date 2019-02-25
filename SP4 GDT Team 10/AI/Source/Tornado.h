#ifndef GAME_TORNADOOBJECT_H
#define GAME_TORNADOOBJECT_H

#include "GameObject.h"

struct Tornado : public GameObject
{
	virtual bool Handle(Message* msg);
	virtual void Update(float dt);
	virtual void TheFunction(GameObject* go);

	Tornado(GAMEOBJECT_TYPE typeValue = GO_NONE);
	~Tornado();

	float fPower; //Strength of the wave
	float fEffectTimer_Cloud;
	float fEffectTimer_Dirt;

	void Collided(GameObject* go);

	std::vector<GameObject*> collidedObjects;
};

#endif