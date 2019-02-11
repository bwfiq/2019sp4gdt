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
		GO_FISH,
		GO_SHARK,
		GO_FISHFOOD,
		GO_BLACK,
		GO_WHITE,

		//Assignment
		GO_CHIEF,
		GO_GUARD,
		GO_CHILDREN,
		GO_ANIMAL,
		GO_ADULT,
		GO_HUNTER,

		GO_NPC,
		GO_CREEP_MELEE,
		GO_CREEP_RANGE,
		GO_TOWER,

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
	int steps;
	float energy;
	float moveSpeed;
	float countDown;
	GameObject *nearest;
	bool moveLeft;
	bool moveRight;
	bool moveUp;
	bool moveDown;


	std::string smID;
	// For shared state machine
	double timer;
	State *m_currState;
	State *m_nextState;

	Node::House side;

	Node* NTarget;

	//Pathfinding
	std::stack<Node*> path;

	bool Handle(Message* msg);

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

	GameObject(GAMEOBJECT_TYPE typeValue = GO_NONE);
	~GameObject();
};

#endif