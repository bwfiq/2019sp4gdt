#ifndef SCENE_GRAPH_H
#define SCENE_GRAPH_H

#include "GameObject.h"
#include <vector>
#include "SceneBase.h"
#include "Maze.h"
#include <queue>
#include "Graph.h"

const float MAX_SPAWN_TIMER = 1.f;

class SceneGraph : public SceneBase
{
public:
	SceneGraph();
	~SceneGraph();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();
	virtual bool Handle(Message* message);

	void RenderGO(GameObject *go);
	GameObject* FetchGO(GameObject::GAMEOBJECT_TYPE type);

	void AStar(GameObject* go, Vector3 target);
	void DFSOnce(GameObject* go);
	void GoRandomAdjacent(GameObject* go);

	void Reset();

	void InitPath();
	void AssignPath(GameObject* go);
	enum PATHS
	{
		RED_TOP,
		RED_MID,
		RED_BOT,
		BLUE_TOP,
		BLUE_MID,
		BLUE_BOT,
		PATH_TOTAL
	};
	std::vector<std::vector<Vector3>>m_paths;

	void RenderGraph();

	enum GAME_STATE
	{
		BLUE_WIN,
		RED_WIN,
		NEUTRAL
	};
protected:

	std::vector<GameObject *> m_goList;
	float m_speed;
	float m_worldWidth;
	float m_worldHeight;
	int m_objectCount;

	unsigned key;
	int numNodes;

	Graph m_graph;

	GameObject* npc;

	float spawnTimer;
	node* redBase;
	node* blueBase;
	GameObject* blueTower;
	GameObject* redTower;

	bool playerControlled;
	int currentSelected;

	GAME_STATE game_state;
};

#endif