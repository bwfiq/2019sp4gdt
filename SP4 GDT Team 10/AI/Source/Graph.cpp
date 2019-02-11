#include "Graph.h"
#include "SceneData.h"

Graph::Graph()
{
	nodeID = 0;
}
Graph::~Graph()
{
	while (!m_nodes.empty())
	{
		node* temp = m_nodes.back();
		m_nodes.pop_back();
		delete temp;
	}
	while (!m_edges.empty())
	{
		edge* temp = m_edges.back();
		m_edges.pop_back();
		delete temp;
	}
}

void Graph::AddNode(Vector3 pos, int ID)
{
	node* m_node = new node(pos);
	m_nodes.push_back(m_node);
	m_node->ID = ID;
}

void Graph::AddNode(node * newNode, int ID)
{
	m_nodes.push_back(newNode);
	newNode->ID = ID;
}

void Graph::AddEdge(node * src, node * dst, float length)
{
	if (src == NULL || dst == NULL)
		return;

	edge* m_edge = new edge(src, dst, length);
	src->edges.push_back(m_edge);
	m_edges.push_back(m_edge);
}

void Graph::CreateNode(Vector3 pos)
{
	// Check for all the other nodes already in the vector
	if (m_nodes.size() != 0)
	{
		for (auto nodes : m_nodes)
		{
			if (nodes->pos == pos)
			{
				//std::cout << "There is already a node existing in that position"<< std::endl;
				return;
			}
		}
	}
	++nodeID;
	node * newNode = new node(pos);
	AddNode(newNode, nodeID);

	//Search around this node for surrounding nodes
	node* theNearestNode = NULL;
	float nearestDistance = FLT_MAX;

	//Check all the nodes around the area
	static float DISTANCE_THRESHOLD = 25.f;
	bool Connected = false;
	for (auto nodesCheck : m_nodes)
	{
		if (newNode == nodesCheck)
			continue;

		Vector3 distance = nodesCheck->pos - newNode->pos;
		float length = distance.LengthSquared();
		if (length < DISTANCE_THRESHOLD * DISTANCE_THRESHOLD)
		{
			AddEdge(newNode, nodesCheck, distance.Length());
			AddEdge(nodesCheck, newNode, distance.Length());
			Connected = true;
		}
		else if (length < nearestDistance)
		{
			theNearestNode = nodesCheck;
			nearestDistance = length;
		}

	}

	//It means that a nearestNode was found
	if (theNearestNode && !Connected)
	{
		AddEdge(newNode, theNearestNode, (newNode->pos - theNearestNode->pos).Length());
		AddEdge(theNearestNode, newNode, (newNode->pos - theNearestNode->pos).Length());
	}
}

node* Graph::NearestNode(const Vector3 & pos)
{
	float distance = FLT_MAX;
	node* curr = NULL;
	for (auto node : m_nodes)
	{
		if ((node->pos - pos).LengthSquared() < distance)
		{
			distance = (node->pos - pos).LengthSquared();
			curr = node;
		}
	}
	return curr;
}



void Graph::Generate(unsigned key, int num_nodes)
{
	nodeID = 0;
	while (!m_nodes.empty())
	{
		node* temp = m_nodes.back();
		m_nodes.pop_back();
		delete temp;
	}
	while (!m_edges.empty())
	{
		edge* temp = m_edges.back();
		m_edges.pop_back();
		delete temp;
	}

	srand(key);
	for (int i = 0; i < num_nodes; ++i)
	{
		float randX = Math::RandFloatMinMax(10, SceneData::GetInstance()->GetWorldHeight() - 10);
		float randY = Math::RandFloatMinMax(10, SceneData::GetInstance()->GetWorldHeight() - 10);
		Vector3 pos(randX, randY, 0);

		bool collide = false;
		for (auto go : m_nodes)
		{
			if ((go->pos - pos).LengthSquared() <= 8 * 8)
			{
				collide = true;
				--i;
				break;
			}
		}

		if (!collide)
		{
			++nodeID;
			AddNode(pos, nodeID);
		}
	}
	for (auto go : m_nodes)
	{
		int randNodes = Math::RandIntMinMax(1, m_nodes.size() - 1);
		while (go->edges.size() < randNodes)
		{
			int index = Math::RandIntMinMax(0, m_nodes.size() - 1);
			node* temp = m_nodes[index];
			if (temp == go)
				continue;
			bool repeated = false;
			for (auto e : go->edges)
			{
				if (e->dst == temp)
				{
					repeated = true;
					break;
				}
			}

			float length = (go->pos - temp->pos).Length();
			AddEdge(go, temp, length);
			AddEdge(temp, go, length);
		}
	}
}
