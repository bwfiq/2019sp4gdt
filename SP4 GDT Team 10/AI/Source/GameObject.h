#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "Vector3.h"
#include "State.h"
#include "ObjectBase.h"
#include "Node.h"
#include <stack>
#include "Maze.h"
#include "Graph.h"

struct GameObject : public ObjectBase
{
	enum GAMEOBJECT_TYPE
	{
		GO_NONE = 0,
		GO_BALL,
		GO_CROSS,
		GO_CIRCLE,

		GO_VILLAGER,
		GO_BUILDING,

		GO_TOTAL, //must be last
	};

	GAMEOBJECT_TYPE type;
	Vector3 pos;
	Vector3 vel;
	Vector3 scale;
	bool active;
	float mass;
	Vector3 target;
	int id;
	float moveSpeed;
	GameObject * goTarget;

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

	virtual bool Handle(Message* msg);

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