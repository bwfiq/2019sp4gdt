#ifndef NODE_H
#define NODE_H

#include "Vector3.h"
#include <vector>
#include <iostream>

struct Node
{
	enum NODE_TYPE
	{
		NODE_WATER,
		NODE_BASE,
		NODE_PATH,
		NODE_TOTAL
	};

	enum House
	{
		TOP_LEFT,
		TOP_RIGHT,
		BOTTOM_LEFT,
		BOTTOM_RIGHT,
		CENTER,
		GUARD,
		TOTAL,
	};
	Node() : pos(NULL), type(NODE_PATH), side(TOP_LEFT)
	{
		combinedWeight = INT_MAX;
		gWeight = INT_MAX;
		inQueue = false;
		parent = NULL;
	}
	Node(Vector3 pos, NODE_TYPE type, House side = TOP_LEFT) : pos(pos), type(type), side(side) 
	{
		combinedWeight = INT_MAX; 
		gWeight = INT_MAX;
		inQueue = false;
		parent = NULL;
	};
	~Node() { connected.clear(); };
	Vector3 pos;
	NODE_TYPE type;
	House side;

	int combinedWeight;
	int gWeight;
	Node* parent;
	bool inQueue;

	friend std::ostream& operator<<(std::ostream& stream, const House& type)
	{
		switch (type)
		{
		case Node::TOP_LEFT:
			stream << "TL House";
			break;
		case Node::TOP_RIGHT:
			stream << "TR House";
			break;
		case Node::BOTTOM_LEFT:
			stream << "BL House";
			break;
		case Node::BOTTOM_RIGHT:
			stream << "BR House";
			break;
		case Node::CENTER:
			stream << "C House";
			break;
		case Node::GUARD:
			stream << "G House";
			break;
		}
		return stream;
	};
	friend std::ostream& operator<<(std::ostream& stream, const Node& type)
	{
		switch (type.type)
		{
		case Node::NODE_WATER:
			stream << "Water";
			break;
		case Node::NODE_BASE:
			stream << type.side;
			break;
		case Node::NODE_PATH:
			stream << "Path";
			break;
		}
		return stream;
	};
	std::vector<std::pair<int, Node*>>connected;
};

struct Compare
{
	bool operator()(Node* node1, Node* node2)
	{
		return node1->combinedWeight < node2->combinedWeight;
	}
};

#endif