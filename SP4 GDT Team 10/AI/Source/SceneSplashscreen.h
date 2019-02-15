#ifndef SCENE_SPLASHSCREEN_H
#define SCENE_SPLASHSCREEN_H

#include "GameObject.h"
#include <vector>
#include "SceneBase.h"
#include <queue>
#include "Graph.h"
#include "MousePicker.h"

#include "Grid.h"

class SceneSplashscreen : public SceneBase
{
public:
	SceneSplashscreen();
	~SceneSplashscreen();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	void Reset();

protected:

	float m_speed;
	float m_worldWidth;
	float m_worldHeight;

	bool bGodlights = false;
	float fTimer = 0;

	Vector3 mousePos;
};

#endif