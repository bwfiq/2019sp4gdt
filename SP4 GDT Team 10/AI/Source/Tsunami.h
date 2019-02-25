#ifndef GAME_TSUNAMIOBJECT_H
#define GAME_TSUNAMIOBJECT_H

#include "GameObject.h"

struct Tsunami : public GameObject
{
	enum TSUNAMI_DIRECTION {
		DIRECTION_UP,
		DIRECTION_DOWN,
		DIRECTION_LEFT,
		DIRECTION_RIGHT,
	};

	TSUNAMI_DIRECTION tsunami_direction;
	virtual bool Handle(Message* msg);
	virtual void Update(float dt);
	virtual void TheFunction(GameObject* go);

	Tsunami(GAMEOBJECT_TYPE typeValue = GO_NONE);
	~Tsunami();

	float fPower; //Strength of the wave
	float fParticleTimer_Cloud;

	void Collided(GameObject* go);

	std::vector<GameObject*> collidedObjects;
};

#endif