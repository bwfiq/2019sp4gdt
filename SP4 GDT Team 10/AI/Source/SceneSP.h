#ifndef SCENE_SP_H
#define SCENE_SP_H

#include "GameObject.h"
#include <vector>
#include "SceneBase.h"
#include "SceneManager.h"
#include <queue>
#include "Graph.h"
#include "MousePicker.h"

#include "Grid.h"

class SceneSP : public SceneBase
{
public:
	enum GAME_STATE
	{
		G_SPLASHSCREEN = 0,
		G_MAINMENU,
		G_INPLAY,

		G_TOTAL
	};
	SceneSP();
	~SceneSP();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();
	virtual bool Handle(Message* message);

	void RenderPassGPass();
	void RenderPassMain();
	void RenderSplashScreen();
	void RenderMainMenu();
	void RenderWorld();

	void RenderGO(GameObject *go);
	GameObject* FetchGO(GameObject::GAMEOBJECT_TYPE type);

	//void AStar(GameObject* go, Vector3 target); //For Graph Pathfinding

	void AStarSingleGrid(GameObject* go, GridPt target); //For Grid Pathfinding with objects that have only 1 grids
	void AStarMultiGrid(GameObject* go, GridPt target); //For Grid Pathfinding with objects that have multiple grids
	bool isTheCoastClear(GameObject* go, GridPt next, Grid::DIRECTION dir); //For making it neat

	void ChangeState(GAME_STATE newstate);
	void ChangeTimeOfDay();
	void ProgressMonth();
	//void DFSOnce(GameObject* go);

	void Reset();

protected:

	std::vector<GameObject *> m_goList;
	float m_speed;
	float m_worldWidth;
	float m_worldHeight;
	int m_objectCount;

	float fSeaDeltaX = 0;
	float fSeaDeltaZ = 0;
	float fSeaDeltaY = 0;
	bool bSeaYDir = false;
	float fTimeOfDay; // 0 to 24
	bool bDay;
	bool bGoalAchieved;
	GAME_STATE game_state;

	bool bGodlights = true;
	float fOpenGLInTimer = -1.f;
	float fOpenGLOutTimer = 2.f;
	float fSplashScreenInTimer = 0.f;
	float fSplashScreenOutTimer = 2.f;
	float fGameStartTimer = 0.5f;

	//Grid
	bool bShowGrid;
	std::vector<Grid::TILE_CONTENT> m_grid;
	std::vector<GridPt> m_shortestPath; //Used for temporary storage of path in AStar
	std::vector<GridPt> m_previous; //Used for storing the previous point in AStar

	Vector3 mousePos;

	GameObject* selected; //Gameobject selected by mouse click, can do actions from UI choices that pop up
	GameObject* goChiefHut;
	GameObject* goAltar;

	//Temporary
	GameObject* goVillager;
	GameObject* goBush;
	GameObject* goTree;
	GameObject* goTree2;
	GameObject* goMountain;
};

#endif