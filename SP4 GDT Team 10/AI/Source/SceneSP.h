#ifndef SCENE_SP_H
#define SCENE_SP_H


#define AABBRAY false

#include "GameObject.h"
#include <vector>
#include "SceneBase.h"
#include "SceneManager.h"
#include <queue>
#include "Graph.h"
#include "MousePicker.h"

#include "Grid.h"
#include "GameSave.h"

class UIBase;
class EffectReticle;
class EffectHand;

class SceneSP : public SceneBase
{
public:
	enum GAME_STATE
	{
		G_SPLASHSCREEN = 0,	// pre-game
		G_MAINMENU,			// pre-game
		G_OPTIONS,			// pre-game
		G_INPLAY,
		G_INGAMEOPTIONS,	// overlay
		G_RESEARCHTREE,		// overlay

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
	void RenderOptions();
	void RenderWorld();

	GameObject* GetHoveredObject();
	bool GetIntersectionAABB(Vector3 lineStart, Vector3 lineEnd, Vector3 minAABB, Vector3 maxAABB, Vector3 &hitPosition);
	bool GetIsIntersecting(const float fDst1, const float fDst2, Vector3 lineStart, Vector3 lineEnd, Vector3 &hitPosition);
	bool InBox(Vector3 hitPosition, Vector3 minAABB, Vector3 maxAABB, const int Axis);

	void RenderGO(GameObject *go);
	GameObject* FetchGO(GameObject::GAMEOBJECT_TYPE type);

	//void AStar(GameObject* go, Vector3 target); //For Graph Pathfinding

	void AStarSingleGrid(GameObject* go, GridPt target); //For Grid Pathfinding with objects that have only 1 grids
	//void AStarMultiGrid(GameObject* go, GridPt target); //For Grid Pathfinding with objects that have multiple grids
	//bool isTheCoastClear(GameObject* go, GridPt next, Grid::DIRECTION dir); //For making it neat

	void ChangeState(GAME_STATE newstate);
	void ChangeTimeOfDay();
	void ProgressMonth();
	//void DFSOnce(GameObject* go);

	void UpdateSelectedUI();

	void Reset();


protected:
	std::vector<UIBase*> m_selectedUi;//this the UI responsible for wat comes up when u select stuff
	std::vector<UIBase*> m_coreUi;//dis da UI that will be up during game states (like aaaaaaaaaaaaa during main menu this will contain the buttons, and during gameplay this will hav UIs that show resources???????)
	std::vector<GameObject *> m_goList;
	EffectReticle* reticle;
	EffectHand* hand;
	float m_speed;
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
	bool bGodMode = false;
	float fOpenGLInTimer = -1.f;
	float fOpenGLOutTimer = 2.f;
	float fSplashScreenInTimer = 0.f;
	float fSplashScreenOutTimer = 2.f;
	float fGameStartTimer = 0.5f;
	float fMainMenuDelta = 1000.f;

	//Grid
	bool bShowGrid;
	std::vector<Grid::TILE_CONTENT> m_grid;
	std::vector<GridPt> m_shortestPath; //Used for temporary storage of path in AStar
	std::vector<GridPt> m_previous; //Used for storing the previous point in AStar

	float fYPos;

	Vector3 mousePos;
	Camera tempCamera;

	//For using ctrl to select and rotate between villagers
	std::vector<GameObject*> m_VillagerList; //List of villagers
	int iCurrItrVillagers;

	bool bWorldEnd;
	float fWaterLevel;

	GameObject* selected; //Gameobject selected by mouse click, can do actions from UI choices that pop up
	GameObject* hovered;
	GameObject* goChiefHut;
	GameObject* goAltar;
	GameObject* goResearchLab;

	//Temporary
	GameObject* goVillager;
	GameObject* goBush;
	GameObject* goTree;
	GameObject* goTree2;
	GameObject* goMountain;
	GameObject* goLogs;
	GameObject* goHouse;

	GameObject* goGranary;
	GameObject* goWoodshed;
	GameObject* goPig;

	GameSave gameSave;
};

#endif