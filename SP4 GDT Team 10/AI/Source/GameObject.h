#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "Vector3.h"
#include "State.h"
#include "ObjectBase.h"
#include "Node.h"
#include <stack>
#include "Grid.h"
#include "Graph.h"
#include "AnimationBase.h"

struct GameObject : public ObjectBase
{
	enum GAMEOBJECT_TYPE
	{
		GO_NONE = 0,
		GO_BALL,
		GO_CROSS,
		GO_CIRCLE,

		GO_VILLAGER,
		GO_PIG,

		GO_ENEMY, //Later

		GO_BUILDING,
		GO_CHIEFHUT,
		GO_ALTAR,
		GO_HOUSE,
		GO_RESEARCHLAB,
		GO_LOGS,
		GO_GRANARY,
		GO_WOODSHED,

		GO_ENVIRONMENT,
		GO_BUSH,
		GO_TREE,
		GO_MOUNTAIN,

		GO_TSUNAMI,
		GO_TORNADO,
		GO_METEOR,

		GO_TOTAL, //must be last
	};

	GAMEOBJECT_TYPE type;
	Vector3 pos;
	Vector3 vel;
	Vector3 scale;
	Vector3 direction;
	bool active;
	float mass;
	Vector3 target;
	int id;
	float moveSpeed;
	GameObject * goTarget;

	//To represent the grids used by the obj
	int iGridX;
	int iGridZ;

	bool bHovered, bSelected;

	/*
	bool moveLeft;
	bool moveRight;
	bool moveUp;
	bool moveDown;*/

	std::string smID;
	// For shared state machine
	double timer;
	State *m_currState;
	State *m_nextState;

	AnimationBase* animation;

	virtual bool Handle(Message* msg);
	virtual void TheFunction(GameObject* go);
	virtual void GiveAnimation(AnimationBase* animation);
	virtual void ClearAnimation(void);
	virtual void Update(float dt);

	int currFrame; //For rendering model animation probs testing

	//For Pathfinding
	std::vector<bool> m_visited;
	std::vector<GridPt> path;

	GridPt currentPt;
	GridPt pickupPt;

	Vector3 minAABB, maxAABB;

	/*
	//Pathfinding
	std::stack<Node*> path;



	bool mapRead;
	bool moving;

	//Maze
	std::vector<Maze::TILE_CONTENT> m_myGrid; //Read and store maze here
	
	std::vector<MazePt> stack;
	std::vector<MazePt> Path;
	MazePt curr;

	//For Graph Travesal
	std::vector<node*> gStack; //Stack for DFSOnce in graph
	node* currNode;
	std::vector<Vector3> gPath; //Vector for current Path
	std::vector<bool> m_visited;
	
	//Assignment 3
	enum FACTION
	{
		FACTION_RED,
		FACTION_BLUE,
		FACTION_NEUTRAL,
		FACTION_TOTAL,
	};

	FACTION faction;
	int health;
	int maxhealth;
	int range;
	int aggroRange;
	int damage;
	float actionTime;

	float attMaxBounceTime;
	float attBounceTime;
	*/
	GameObject(GAMEOBJECT_TYPE typeValue = GO_NONE);
	~GameObject();
};

#endif