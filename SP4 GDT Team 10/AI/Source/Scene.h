#ifndef SCENE_H
#define SCENE_H

#include "ObjectBase.h"

class Scene : public ObjectBase
{
public:
	Scene() {}
	virtual ~Scene() {}

	virtual void Init() = 0;
	virtual void Update(double dt) = 0;
	virtual void Render() = 0;
	virtual void Exit() = 0;
	virtual bool Handle(Message* msg) = 0;
};

#endif