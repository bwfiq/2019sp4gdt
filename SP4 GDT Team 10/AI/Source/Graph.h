#ifndef GRAPH_H
#define GRAPH_H
#include "Vector3.h"
#include <vector>
struct node;
struct edge
{
	node* src;
	node* dst;

	float length;
	
	edge() 
	{ 
		src = NULL;
		dst = NULL;
		length = 0.f;
	}
	edge(float length) : length(length)
	{
		src = NULL;
		dst = NULL;
	}
	edge(node* src, node* dst, float length) : src(src), dst(dst), length(length)
	{
	}
};

struct node
{
	Vector3 pos;
	std::vector<edge*> edges;
	node* previous = NULL;
	node()
	{
		pos.Set(0, 0, 0);
		ID = 0;
	}
	node(Vector3 pos) : pos(pos)
	{
		ID = 0;
	}
	int ID;
};

class Graph
{
public:

	std::vector<node*> m_nodes;
	std::vector<edge*> m_edges;

	Graph();
	~Graph();
	int nodeID;
	void AddNode(Vector3 pos, int ID);
	void AddNode(node* newNode, int ID);
	void AddEdge(node* node1, node* node2, float length);
	void CreateNode(Vector3 pos);
	node* NearestNode(const Vector3& pos);

	void Generate(unsigned key, int num_nodes);
};

#endif