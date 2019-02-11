#include "SceneGraph.h"
#include "GL\glew.h"
#include "Application.h"
#include <sstream>
#include "SceneData.h"

#include "SceneData.h"
#include "PostOffice.h"
#include "ConcreteMessages.h"
#include "ProjectileManager.h"

#include "SMManager.h"

#define START_PLAYER false
SceneGraph::SceneGraph()
{
}

SceneGraph::~SceneGraph()
{
}

void SceneGraph::Init()
{
	SceneBase::Init();

	//Calculating aspect ratio
	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();
	SceneData::GetInstance()->SetWorldHeight(m_worldHeight);
	SceneData::GetInstance()->SetWorldWidth(m_worldWidth);
	PostOffice::GetInstance()->Register("Scene", this);

	//Physics code here
	m_speed = 1.f;

	Math::InitRNG();

	key = 0;
	numNodes = 10;

	spawnTimer = MAX_SPAWN_TIMER;
	InitPath();

	playerControlled = START_PLAYER;
	currentSelected = BLUE_TOP;

	game_state = NEUTRAL;
	//m_graph.Generate(key, numNodes);
}


bool SceneGraph::Handle(Message* message)
{

	MessageWRU* messageWRU = dynamic_cast<MessageWRU*>(message);
	if (messageWRU)
	{
		switch (messageWRU->type)
		{
		case MessageWRU::NEAREST_SHARK:
		{
			float nearestDistance = FLT_MAX;
			for (auto go2 : m_goList)
			{
				if (!go2->active || go2->type != GameObject::GO_SHARK)
					continue;
				float distance = (messageWRU->go->pos - go2->pos).Length();
				if (distance < messageWRU->threshold && distance < nearestDistance && messageWRU->go->m_currState->GetStateID() == "Full")
				{
					nearestDistance = distance;
					messageWRU->go->nearest = go2;
				}
			}
		}
		break;
		case MessageWRU::NEAREST_ENEMY:
		{
			float nearestDistance = FLT_MAX;
			for (auto go2 : m_goList)
			{
				if (!go2->active || go2->faction == messageWRU->go->faction)
					continue;
				float distance = (messageWRU->go->pos - go2->pos).Length();
				if (distance <= messageWRU->threshold && distance < nearestDistance)
				{
					nearestDistance = distance;
					messageWRU->go->nearest = go2;
				}
			}
		}
		break;
		}
		delete message;
		return true;
	}
	delete message;
	return false;
}


GameObject* SceneGraph::FetchGO(GameObject::GAMEOBJECT_TYPE type)
{
	for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if (!go->active && go->type == type)
		{
			go->active = true;
			++m_objectCount;
			switch (type)
			{
			case GameObject::GO_TOWER:
				go->attBounceTime = 0;
				go->attMaxBounceTime = 1.5f;
				go->health = 300;
				go->maxhealth = 300;
				go->scale.Set(7.f, 7.f, 1.f);
				go->damage = 40;

				go->range = 2.5 * go->scale.x;
				go->aggroRange = 2.5 * go->scale.x;
				break;
			case GameObject::GO_CREEP_MELEE:
				go->attBounceTime = 0;
				go->attMaxBounceTime = 0.8f;
				go->health = 100;
				go->maxhealth = 100;
				go->scale.Set(7.f, 7.f, 1.f);
				go->damage = 10;

				go->range = 1 * go->scale.x;
				go->aggroRange = 3 * go->scale.x;
				break;
			case GameObject::GO_CREEP_RANGE:
				go->attBounceTime = 0;
				go->attMaxBounceTime = 1.5f;
				go->health = 60;
				go->maxhealth = 60;
				go->scale.Set(7.f, 7.f, 1.f);
				go->damage = 30;

				go->range = 1.5 * go->scale.x;
				go->aggroRange = 3 * go->scale.x;
				break;
			}

			go->nearest = NULL;
			return go;
		}
	}
	for (unsigned i = 0; i < 5; ++i)
	{
		GameObject *go = new GameObject(type);
		m_goList.push_back(go);
	}
	return FetchGO(type);
}

void SceneGraph::AStar(GameObject * go, Vector3 target)
{
	go->m_visited.resize(m_graph.nodeID);
	std::fill(go->m_visited.begin(), go->m_visited.end(), false);
	go->gPath.clear();
	node* nearestToTarget = m_graph.NearestNode(target);

	//node* is the current node, first float is cost to node, second float is cost to node + distance to end
	std::vector<std::pair<node*, std::pair<float, float>>> m_queue;
	m_queue.push_back(std::pair<node*, std::pair<float, float>>(go->currNode, std::pair<float, float>(0.f, (nearestToTarget->pos - go->currNode->pos).LengthSquared())));
	while (m_queue.front().first != nearestToTarget && !m_queue.empty())
	{

		std::pair<node*, std::pair<float, float>> curr = m_queue.front();
		node* currNode = curr.first;

		for (auto edges : currNode->edges)
		{
			float cost = curr.second.first + edges->length;
			float costToEnd = cost + (nearestToTarget->pos - edges->dst->pos).LengthSquared();
			std::pair<node*, std::pair<float, float>> value(edges->dst, std::pair<float, float>(cost, costToEnd));
			if (!go->m_visited[edges->dst->ID - 1])
			{
				edges->dst->previous = currNode;
				go->m_visited[edges->dst->ID - 1] = true;
				bool inserted = false;
				for (std::vector<std::pair<node*, std::pair<float, float>>>::iterator it = m_queue.begin(); it != m_queue.end(); ++it)
				{
					std::pair<node*, std::pair<float, float>> temp = *it;
					if (value.second.second < temp.second.second)
					{
						m_queue.insert(it,value);
						inserted = true;
						break;
					}
				}
				if (!inserted)
				{
					m_queue.push_back(value);
				}
			}
			else
			{
				for (auto go : m_queue)
				{
					if (go.first == value.first)
					{
						edges->dst->previous = currNode;
						go = value;
						break;
					}
				}
			}
		}
		std::vector<std::pair<node*, std::pair<float, float>>>::iterator tempit;
		tempit = std::find(m_queue.begin(), m_queue.end(), curr);
		m_queue.erase(tempit);
	}

	node* curr = nearestToTarget;
	while (curr != go->currNode)
	{
		go->gPath.push_back(curr->pos);
		curr = curr->previous;
	}
}

void SceneGraph::DFSOnce(GameObject * go)
{
	go->gStack.push_back(go->currNode);
	go->m_visited[go->currNode->ID - 1] = true;
	for (auto edge : go->currNode->edges)
	{
		if (!go->m_visited[edge->dst->ID - 1])
		{
			go->currNode = edge->dst;
			return;
		}
	}
	go->gStack.pop_back();
	if (!go->gStack.empty())
	{
		go->currNode = go->gStack.back();
		go->gStack.pop_back();
	}
	return;
}

void SceneGraph::Reset()
{	
	//Cleanup GameObjects
	while (m_goList.size() > 0)
	{
		GameObject *go = m_goList.back();
		delete go;
		m_goList.pop_back();
	}

	m_speed = 1.f;
	InitPath();

	game_state = NEUTRAL;
	spawnTimer = MAX_SPAWN_TIMER;
}

void SceneGraph::InitPath()
{
	m_paths.clear();
	m_paths.resize(PATH_TOTAL);
	m_graph.nodeID = 1;
	while (!m_graph.m_nodes.empty())
	{
		node* temp = m_graph.m_nodes.back();
		m_graph.m_nodes.pop_back();
		delete temp;
	}
	while (!m_graph.m_edges.empty())
	{
		edge* temp = m_graph.m_edges.back();
		m_graph.m_edges.pop_back();
		delete temp;
	}

	float m_worldWidth = SceneData::GetInstance()->GetWorldWidth();
	float m_worldHeight = SceneData::GetInstance()->GetWorldHeight();

	GameObject* go = nullptr;

	Vector3 currPos(m_worldHeight * 0.08f, m_worldHeight * 0.04f);
	blueBase = new node(currPos);
	m_graph.AddNode(blueBase, m_graph.nodeID);
	++m_graph.nodeID;
	go = FetchGO(GameObject::GO_TOWER);
	go->faction = GameObject::FACTION_BLUE;
	go->currNode = blueBase;
	go->pos = blueBase->pos;
	//go->smID = "TowerSM";
	//go->m_currState = go->m_nextState = SMManager::GetInstance()->GetSM(go->smID)->GetState("Wait");
	blueTower = go;

	//Generating path and graph
	//Top Lane
	currPos.Set(m_worldHeight * 0.14f, m_worldHeight * 0.3f, 0);
	node* t1 = new node(currPos);
	m_graph.AddNode(t1, m_graph.nodeID);
	++m_graph.nodeID;
	go = FetchGO(GameObject::GO_TOWER);
	go->faction = GameObject::FACTION_BLUE;
	go->currNode = t1;
	go->pos = t1->pos;
	//go->smID = "TowerSM";
	//go->m_currState = go->m_nextState = SMManager::GetInstance()->GetSM(go->smID)->GetState("Wait");

	currPos.Set(m_worldHeight * 0.15f, m_worldHeight * 0.47f, 0);
	node* t2 = new node(currPos);
	m_graph.AddNode(t2, m_graph.nodeID);
	++m_graph.nodeID;
	go = FetchGO(GameObject::GO_TOWER);
	go->faction = GameObject::FACTION_BLUE;
	go->currNode = t2;
	go->pos = t2->pos;
	//go->smID = "TowerSM";
	//go->m_currState = go->m_nextState = SMManager::GetInstance()->GetSM(go->smID)->GetState("Wait");

	currPos.Set(m_worldHeight * 0.16f, m_worldHeight * 0.65f, 0);
	node* t3 = new node(currPos);
	m_graph.AddNode(t3, m_graph.nodeID);
	++m_graph.nodeID;
	go = FetchGO(GameObject::GO_TOWER);
	go->faction = GameObject::FACTION_BLUE;
	go->currNode = t3;
	go->pos = t3->pos;
	//go->smID = "TowerSM";
	//go->m_currState = go->m_nextState = SMManager::GetInstance()->GetSM(go->smID)->GetState("Wait");

	currPos.Set(m_worldHeight * 0.20f, m_worldHeight * 0.76f, 0);
	node* t4 = new node(currPos);
	m_graph.AddNode(t4, m_graph.nodeID);
	++m_graph.nodeID;

	currPos.Set(m_worldHeight * 0.26f, m_worldHeight * 0.81f, 0);
	node* t5 = new node(currPos);
	m_graph.AddNode(t5, m_graph.nodeID);
	++m_graph.nodeID;

	currPos.Set(m_worldHeight * 0.38f, m_worldHeight * 0.855f, 0);
	node* t6 = new node(currPos);
	m_graph.AddNode(t6, m_graph.nodeID);
	++m_graph.nodeID;
	go = FetchGO(GameObject::GO_TOWER);
	go->faction = GameObject::FACTION_RED;
	go->currNode = t6;
	go->pos = t6->pos;
	//go->smID = "TowerSM";
	//go->m_currState = go->m_nextState = SMManager::GetInstance()->GetSM(go->smID)->GetState("Wait");

	currPos.Set(m_worldHeight * 0.57f, m_worldHeight * 0.865f, 0);
	node* t7 = new node(currPos);
	m_graph.AddNode(t7, m_graph.nodeID);
	++m_graph.nodeID;
	go = FetchGO(GameObject::GO_TOWER);
	go->faction = GameObject::FACTION_RED;
	go->currNode = t7;
	go->pos = t7->pos;
	//go->smID = "TowerSM";
	//go->m_currState = go->m_nextState = SMManager::GetInstance()->GetSM(go->smID)->GetState("Wait");

	currPos.Set(m_worldHeight * 0.72f, m_worldHeight * 0.88f, 0);
	node* t8 = new node(currPos);
	m_graph.AddNode(t8, m_graph.nodeID);
	++m_graph.nodeID;
	go = FetchGO(GameObject::GO_TOWER);
	go->faction = GameObject::FACTION_RED;
	go->currNode = t8;
	go->pos = t8->pos;
	//go->smID = "TowerSM";
	//go->m_currState = go->m_nextState = SMManager::GetInstance()->GetSM(go->smID)->GetState("Wait");


	//Mid Lane
	currPos.Set(m_worldHeight * 0.287f, m_worldHeight * 0.27f, 0);
	node* m1 = new node(currPos);
	m_graph.AddNode(m1, m_graph.nodeID);
	++m_graph.nodeID;
	go = FetchGO(GameObject::GO_TOWER);
	go->faction = GameObject::FACTION_BLUE;
	go->currNode = m1;
	go->pos = m1->pos;
	//go->smID = "TowerSM";
	//go->m_currState = go->m_nextState = SMManager::GetInstance()->GetSM(go->smID)->GetState("Wait");

	currPos.Set(m_worldHeight * 0.39f, m_worldHeight * 0.34f, 0);
	node* m2 = new node(currPos);
	m_graph.AddNode(m2, m_graph.nodeID);
	++m_graph.nodeID;
	go = FetchGO(GameObject::GO_TOWER);
	go->faction = GameObject::FACTION_BLUE;
	go->currNode = m2;
	go->pos = m2->pos;
	//go->smID = "TowerSM";
	//go->m_currState = go->m_nextState = SMManager::GetInstance()->GetSM(go->smID)->GetState("Wait");

	currPos.Set(m_worldHeight * 0.48f, m_worldHeight * 0.445f, 0);
	node* m3 = new node(currPos);
	m_graph.AddNode(m3, m_graph.nodeID);
	++m_graph.nodeID;
	go = FetchGO(GameObject::GO_TOWER);
	go->faction = GameObject::FACTION_BLUE;
	go->currNode = m3;
	go->pos = m3->pos;
	//go->smID = "TowerSM";
	//go->m_currState = go->m_nextState = SMManager::GetInstance()->GetSM(go->smID)->GetState("Wait");

	currPos.Set(m_worldHeight * 0.61f, m_worldHeight * 0.56f, 0);
	node* m4 = new node(currPos);
	m_graph.AddNode(m4, m_graph.nodeID);
	++m_graph.nodeID;
	currPos.Set(m_worldHeight * 0.7f, m_worldHeight * 0.65f, 0);
	go = FetchGO(GameObject::GO_TOWER);
	go->faction = GameObject::FACTION_RED;
	go->currNode = m4;
	go->pos = m4->pos;
	//go->smID = "TowerSM";
	//go->m_currState = go->m_nextState = SMManager::GetInstance()->GetSM(go->smID)->GetState("Wait");

	node* m5 = new node(currPos);
	m_graph.AddNode(m5, m_graph.nodeID);
	++m_graph.nodeID;
	currPos.Set(m_worldHeight * 0.78f, m_worldHeight * 0.73f, 0);
	go = FetchGO(GameObject::GO_TOWER);
	go->faction = GameObject::FACTION_RED;
	go->currNode = m5;
	go->pos = m5->pos;
	//go->smID = "TowerSM";
	//go->m_currState = go->m_nextState = SMManager::GetInstance()->GetSM(go->smID)->GetState("Wait");

	node* m6 = new node(currPos);
	m_graph.AddNode(m6, m_graph.nodeID);
	++m_graph.nodeID;
	go = FetchGO(GameObject::GO_TOWER);
	go->faction = GameObject::FACTION_RED;
	go->currNode = m6;
	go->pos = m6->pos;
	//go->smID = "TowerSM";
	//go->m_currState = go->m_nextState = SMManager::GetInstance()->GetSM(go->smID)->GetState("Wait");


	//Bot Lane
	currPos.Set(m_worldHeight * 0.36f, m_worldHeight * 0.08f, 0);
	node* b1 = new node(currPos);
	m_graph.AddNode(b1, m_graph.nodeID);
	++m_graph.nodeID;
	go = FetchGO(GameObject::GO_TOWER);
	go->faction = GameObject::FACTION_BLUE;
	go->currNode = b1;
	go->pos = b1->pos;
	//go->smID = "TowerSM";
	//go->m_currState = go->m_nextState = SMManager::GetInstance()->GetSM(go->smID)->GetState("Wait");

	currPos.Set(m_worldHeight * 0.53f, m_worldHeight * 0.09f, 0);
	node* b2 = new node(currPos);
	m_graph.AddNode(b2, m_graph.nodeID);
	++m_graph.nodeID;
	go = FetchGO(GameObject::GO_TOWER);
	go->faction = GameObject::FACTION_BLUE;
	go->currNode = b2;
	go->pos = b2->pos;
	//go->smID = "TowerSM";
	//go->m_currState = go->m_nextState = SMManager::GetInstance()->GetSM(go->smID)->GetState("Wait");

	currPos.Set(m_worldHeight * 0.73f, m_worldHeight * 0.11f, 0);
	node* b3 = new node(currPos);
	m_graph.AddNode(b3, m_graph.nodeID);
	++m_graph.nodeID;
	go = FetchGO(GameObject::GO_TOWER);
	go->faction = GameObject::FACTION_BLUE;
	go->currNode = b3;
	go->pos = b3->pos;
	//go->smID = "TowerSM";
	//go->m_currState = go->m_nextState = SMManager::GetInstance()->GetSM(go->smID)->GetState("Wait");

	currPos.Set(m_worldHeight * 0.83f, m_worldHeight * 0.14f, 0);
	node* b4 = new node(currPos);
	m_graph.AddNode(b4, m_graph.nodeID);
	++m_graph.nodeID;
	currPos.Set(m_worldHeight * 0.92f, m_worldHeight * 0.2f, 0);
	node* b5 = new node(currPos);
	m_graph.AddNode(b5, m_graph.nodeID);
	++m_graph.nodeID;
	currPos.Set(m_worldHeight * 0.93f, m_worldHeight * 0.31f, 0);
	node* b6 = new node(currPos);
	m_graph.AddNode(b6, m_graph.nodeID);
	++m_graph.nodeID;
	currPos.Set(m_worldHeight * 0.935f, m_worldHeight * 0.56f, 0);
	go = FetchGO(GameObject::GO_TOWER);
	go->faction = GameObject::FACTION_RED;
	go->currNode = b6;
	go->pos = b6->pos;
	//go->smID = "TowerSM";
	//go->m_currState = go->m_nextState = SMManager::GetInstance()->GetSM(go->smID)->GetState("Wait");

	node* b7 = new node(currPos);
	m_graph.AddNode(b7, m_graph.nodeID);
	++m_graph.nodeID;
	currPos.Set(m_worldHeight * 0.94f, m_worldHeight * 0.7f, 0);
	go = FetchGO(GameObject::GO_TOWER);
	go->faction = GameObject::FACTION_RED;
	go->currNode = b7;
	go->pos = b7->pos;
	//go->smID = "TowerSM";
	//go->m_currState = go->m_nextState = SMManager::GetInstance()->GetSM(go->smID)->GetState("Wait");

	node* b8 = new node(currPos);
	m_graph.AddNode(b8, m_graph.nodeID);
	++m_graph.nodeID;
	go = FetchGO(GameObject::GO_TOWER);
	go->faction = GameObject::FACTION_RED;
	go->currNode = b8;
	go->pos = b8->pos;
	//go->smID = "TowerSM";
	//go->m_currState = go->m_nextState = SMManager::GetInstance()->GetSM(go->smID)->GetState("Wait");
	
	currPos.Set(m_worldHeight * 0.95f, m_worldHeight * 0.90f, 0);
	redBase = new node(currPos);
	m_graph.AddNode(redBase, m_graph.nodeID);
	++m_graph.nodeID;
	go = FetchGO(GameObject::GO_TOWER);
	go->faction = GameObject::FACTION_RED;
	go->currNode = redBase;
	go->pos = redBase->pos;
	//go->smID = "TowerSM";
	//go->m_currState = go->m_nextState = SMManager::GetInstance()->GetSM(go->smID)->GetState("Wait");
	redTower = go;

	//Adding the edges
	m_graph.AddEdge(blueBase, t1, (blueBase->pos - t1->pos).Length());
	m_graph.AddEdge(t1, blueBase, (blueBase->pos - t1->pos).Length());
	m_graph.AddEdge(blueBase, m1, (blueBase->pos - m1->pos).Length());
	m_graph.AddEdge(m1, blueBase, (blueBase->pos - m1->pos).Length());
	m_graph.AddEdge(blueBase, b1, (blueBase->pos - b1->pos).Length());
	m_graph.AddEdge(b1, blueBase, (blueBase->pos - b1->pos).Length());

	m_graph.AddEdge(t1, t2, (t1->pos - t2->pos).Length());
	m_graph.AddEdge(t2, t1, (t1->pos - t2->pos).Length());
	m_graph.AddEdge(m1, m2, (m1->pos - m2->pos).Length());
	m_graph.AddEdge(m2, m1, (m1->pos - m2->pos).Length());
	m_graph.AddEdge(b1, b2, (b1->pos - b2->pos).Length());
	m_graph.AddEdge(b2, b1, (b1->pos - b2->pos).Length());

	m_graph.AddEdge(t3, t2, (t3->pos - t2->pos).Length());
	m_graph.AddEdge(t2, t3, (t3->pos - t2->pos).Length());
	m_graph.AddEdge(m3, m2, (m3->pos - m2->pos).Length());
	m_graph.AddEdge(m2, m3, (m3->pos - m2->pos).Length());
	m_graph.AddEdge(b3, b2, (b3->pos - b2->pos).Length());
	m_graph.AddEdge(b2, b3, (b3->pos - b2->pos).Length());

	m_graph.AddEdge(t3, t4, (t3->pos - t4->pos).Length());
	m_graph.AddEdge(t4, t3, (t3->pos - t4->pos).Length());
	m_graph.AddEdge(m3, m4, (m3->pos - m4->pos).Length());
	m_graph.AddEdge(m4, m3, (m3->pos - m4->pos).Length());
	m_graph.AddEdge(b3, b4, (b3->pos - b4->pos).Length());
	m_graph.AddEdge(b4, b3, (b3->pos - b4->pos).Length());

	m_graph.AddEdge(t4, t5, (t4->pos - t5->pos).Length());
	m_graph.AddEdge(t5, t4, (t4->pos - t5->pos).Length());
	m_graph.AddEdge(m4, m5, (m4->pos - m5->pos).Length());
	m_graph.AddEdge(m5, m4, (m4->pos - m5->pos).Length());
	m_graph.AddEdge(b4, b5, (b4->pos - b5->pos).Length());
	m_graph.AddEdge(b5, b4, (b4->pos - b5->pos).Length());

	m_graph.AddEdge(t5, t6, (t5->pos - t6->pos).Length());
	m_graph.AddEdge(t6, t5, (t5->pos - t6->pos).Length());
	m_graph.AddEdge(m5, m6, (m5->pos - m6->pos).Length());
	m_graph.AddEdge(m6, m5, (m5->pos - m6->pos).Length());
	m_graph.AddEdge(b5, b6, (b5->pos - b6->pos).Length());
	m_graph.AddEdge(b6, b5, (b5->pos - b6->pos).Length());

	m_graph.AddEdge(t7, t6, (t7->pos - t6->pos).Length());
	m_graph.AddEdge(t6, t7, (t7->pos - t6->pos).Length());
	m_graph.AddEdge(b7, b6, (b7->pos - b6->pos).Length());
	m_graph.AddEdge(b6, b7, (b7->pos - b6->pos).Length());

	m_graph.AddEdge(t7, t8, (t7->pos - t8->pos).Length());
	m_graph.AddEdge(t8, t7, (t7->pos - t8->pos).Length());
	m_graph.AddEdge(b7, b8, (b7->pos - b8->pos).Length());
	m_graph.AddEdge(b8, b7, (b7->pos - b8->pos).Length());

	m_graph.AddEdge(redBase, t8, (redBase->pos - t8->pos).Length());
	m_graph.AddEdge(t8, redBase, (redBase->pos - t8->pos).Length());
	m_graph.AddEdge(redBase, m6, (redBase->pos - m6->pos).Length());
	m_graph.AddEdge(m6, redBase, (redBase->pos - m6->pos).Length());
	m_graph.AddEdge(redBase, b8, (redBase->pos - b8->pos).Length());
	m_graph.AddEdge(b8, redBase, (redBase->pos - b8->pos).Length());

	//Initing preset paths
	m_paths[RED_TOP].push_back(blueBase->pos);
	m_paths[RED_TOP].push_back(t1->pos);
	m_paths[RED_TOP].push_back(t2->pos);
	m_paths[RED_TOP].push_back(t3->pos);
	m_paths[RED_TOP].push_back(t4->pos);
	m_paths[RED_TOP].push_back(t5->pos);
	m_paths[RED_TOP].push_back(t6->pos);
	m_paths[RED_TOP].push_back(t7->pos);
	m_paths[RED_TOP].push_back(t8->pos);
	m_paths[RED_TOP].push_back(redBase->pos);


	m_paths[RED_MID].push_back(blueBase->pos);
	m_paths[RED_MID].push_back(m1->pos);
	m_paths[RED_MID].push_back(m2->pos);
	m_paths[RED_MID].push_back(m3->pos);
	m_paths[RED_MID].push_back(m4->pos);
	m_paths[RED_MID].push_back(m5->pos);
	m_paths[RED_MID].push_back(m6->pos);
	m_paths[RED_MID].push_back(redBase->pos);

	m_paths[RED_BOT].push_back(blueBase->pos);
	m_paths[RED_BOT].push_back(b1->pos);
	m_paths[RED_BOT].push_back(b2->pos);
	m_paths[RED_BOT].push_back(b3->pos);
	m_paths[RED_BOT].push_back(b4->pos);
	m_paths[RED_BOT].push_back(b5->pos);
	m_paths[RED_BOT].push_back(b6->pos);
	m_paths[RED_BOT].push_back(b7->pos);
	m_paths[RED_BOT].push_back(b8->pos);
	m_paths[RED_BOT].push_back(redBase->pos);

	m_paths[BLUE_TOP].push_back(redBase->pos);
	m_paths[BLUE_TOP].push_back(t8->pos);
	m_paths[BLUE_TOP].push_back(t7->pos);
	m_paths[BLUE_TOP].push_back(t6->pos);
	m_paths[BLUE_TOP].push_back(t5->pos);
	m_paths[BLUE_TOP].push_back(t4->pos);
	m_paths[BLUE_TOP].push_back(t3->pos);
	m_paths[BLUE_TOP].push_back(t2->pos);
	m_paths[BLUE_TOP].push_back(t1->pos);
	m_paths[BLUE_TOP].push_back(blueBase->pos);

	m_paths[BLUE_MID].push_back(redBase->pos);
	m_paths[BLUE_MID].push_back(m6->pos);
	m_paths[BLUE_MID].push_back(m5->pos);
	m_paths[BLUE_MID].push_back(m4->pos);
	m_paths[BLUE_MID].push_back(m3->pos);
	m_paths[BLUE_MID].push_back(m2->pos);
	m_paths[BLUE_MID].push_back(m1->pos);
	m_paths[BLUE_MID].push_back(blueBase->pos);

	m_paths[BLUE_BOT].push_back(redBase->pos);
	m_paths[BLUE_BOT].push_back(b8->pos);
	m_paths[BLUE_BOT].push_back(b7->pos);
	m_paths[BLUE_BOT].push_back(b6->pos);
	m_paths[BLUE_BOT].push_back(b5->pos);
	m_paths[BLUE_BOT].push_back(b4->pos);
	m_paths[BLUE_BOT].push_back(b3->pos);
	m_paths[BLUE_BOT].push_back(b2->pos);
	m_paths[BLUE_BOT].push_back(b1->pos);
	m_paths[BLUE_BOT].push_back(blueBase->pos);
}

void SceneGraph::AssignPath(GameObject * go)
{
	if (go->faction == GameObject::FACTION_RED)
	{
		go->gPath = m_paths[Math::RandIntMinMax((int)RED_TOP, (int)RED_BOT)];
		go->target = go->gPath.back();
	}
	else if (go->faction == GameObject::FACTION_BLUE)
	{
		if(!playerControlled)
			go->gPath = m_paths[Math::RandIntMinMax((int)BLUE_TOP, (int)BLUE_BOT)];
		else
			go->gPath = m_paths[currentSelected];
		go->target = go->gPath.back();
	}
	else
	{
		go->gPath = m_paths[Math::RandIntMinMax(0, PATH_TOTAL - 1)];
		go->target = go->gPath.back();
	}
}

void SceneGraph::Update(double dt)
{
	if (Application::IsKeyPressed('R'))
	{
		Reset();
		Sleep(100);
		return;
	}

	if (game_state != NEUTRAL)
		return;

	SceneBase::Update(dt);

	//Calculating aspect ratio
	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();

	SceneData::GetInstance()->SetWorldHeight(m_worldHeight);
	SceneData::GetInstance()->SetWorldWidth(m_worldWidth);

	if (Application::IsKeyPressed(VK_OEM_MINUS))
	{
		m_speed = Math::Max(0.f, m_speed - 0.1f);
	}
	if (Application::IsKeyPressed(VK_OEM_PLUS))
	{
		m_speed += 0.1f;
	}
	if (Application::IsKeyPressed('N'))
	{
	}
	if (Application::IsKeyPressed('M'))
	{
	}
	if (Application::IsKeyPressed(VK_RETURN))
	{
	}

	//Input Section
	static bool bPState = false;
	if (!bPState && Application::IsKeyPressed('P'))
	{
		bPState = true;
		playerControlled = !playerControlled;
	}
	else if(bPState && !Application::IsKeyPressed('P'))
	{
		bPState = false;
		std::cout << "P UP" << std::endl;
	}
	if (playerControlled)
	{
		static bool b1State = false;
		if (!b1State && Application::IsKeyPressed('1'))
		{
			b1State = true;
			currentSelected = BLUE_TOP;
		}
		else if (b1State && !Application::IsKeyPressed('1'))
		{
			b1State = false;
			std::cout << "P UP" << std::endl;
		}

		static bool b2State = false;
		if (!b2State && Application::IsKeyPressed('2'))
		{
			b2State = true;
			currentSelected = BLUE_MID
;
		}
		else if (b2State && !Application::IsKeyPressed('2'))
		{
			b2State = false;
			std::cout << "P UP" << std::endl;
		}

		static bool b3State = false;
		if (!b3State && Application::IsKeyPressed('3'))
		{
			b3State = true;
			currentSelected = BLUE_BOT;
		}
		else if (b3State && !Application::IsKeyPressed('3'))
		{
			b3State = false;
			std::cout << "P UP" << std::endl;
		}
	}

	static bool bLButtonState = false;
	if (!bLButtonState && Application::IsMousePressed(0))
	{
		bLButtonState = true;
		std::cout << "LBUTTON DOWN" << std::endl;
		double x, y;
		Application::GetCursorPos(&x, &y);
		int w = Application::GetWindowWidth();
		int h = Application::GetWindowHeight();
		float posX = static_cast<float>(x) / w * m_worldWidth;
		float posY = (h - static_cast<float>(y)) / h * m_worldHeight;
		if (posX < m_worldHeight && posY < m_worldHeight)
		{
		//	m_graph.CreateNode(Vector3(posX, posY));
		}
	}
	else if (bLButtonState && !Application::IsMousePressed(0))
	{
		bLButtonState = false;
		std::cout << "LBUTTON UP" << std::endl;
	}
	Vector3 clickTarget = NULL;
	static bool bRButtonState = false;
	if (!bRButtonState && Application::IsMousePressed(1))
	{
		bRButtonState = true;
		std::cout << "RBUTTON DOWN" << std::endl;

		double x, y;
		Application::GetCursorPos(&x, &y);
		int w = Application::GetWindowWidth();
		int h = Application::GetWindowHeight();
		float posX = static_cast<float>(x) / w * m_worldWidth;
		float posY = (h - static_cast<float>(y)) / h * m_worldHeight;
		if (posX < m_worldHeight && posY < m_worldHeight)
		{
			clickTarget.Set(posX, posY, 0);
		}

	}
	else if (bRButtonState && !Application::IsMousePressed(1))
	{
		bRButtonState = false;
		std::cout << "RBUTTON UP" << std::endl;
	}
	static bool bSpaceState = false;
	if (!bSpaceState && Application::IsKeyPressed(VK_SPACE))
	{
		bSpaceState = true;

		double x, y;
		Application::GetCursorPos(&x, &y);
		int w = Application::GetWindowWidth();
		int h = Application::GetWindowHeight();
		float posX = static_cast<float>(x) / w * m_worldWidth;
		float posY = (h - static_cast<float>(y)) / h * m_worldHeight;
		if (posX < m_worldHeight && posY < m_worldHeight)
		{
			GameObject* go = FetchGO(GameObject::GO_NPC);
			go->pos.Set(posX, posY);
			node* nearestNode = m_graph.NearestNode(go->pos);
			if (nearestNode != NULL)
			{
				go->target = nearestNode->pos;
				go->currNode = nearestNode;
			}
			else
			{
				go->target == NULL;
			}
			go->mapRead = false;
		}
	}
	else if (bSpaceState && !Application::IsKeyPressed(VK_SPACE))
	{
		bSpaceState = false;
	}

	//Update WinLose
	if (!blueTower->active)
	{
		game_state = RED_WIN;
		return;
	}
	else if (!redTower->active)
	{
		game_state = BLUE_WIN;
		return;
	}

	//Spawning minions
	bool spawning = false;
	//Could be changed
	static const int redMinionSpawn[] = { 1, 1, 1, 2, 2, 2 };
	static const int blueMinionSpawn[] = { 1, 1, 1, 2, 2, 2 };
	static int redSpawnCount = 0;
	static int blueSpawnCount = 0;
	spawnTimer -= dt;
	if (spawnTimer <= 0.f)
	{
		spawning = true;
		spawnTimer = MAX_SPAWN_TIMER;
	}
	if (spawning)
	{
		GameObject* go = nullptr;
		switch (redMinionSpawn[redSpawnCount])
		{
		case 1:
			go = FetchGO(GameObject::GO_CREEP_MELEE);
			break;
		case 2:
			go = FetchGO(GameObject::GO_CREEP_RANGE);
			break;
		}
		redSpawnCount = (redSpawnCount + 1) % 6;
		go->nearest = NULL;
		go->faction = GameObject::FACTION_RED;
		go->currNode = redBase;
		go->pos = redBase->pos;
		AssignPath(go);
		//go->smID = "MinionSM";
		//go->m_currState = go->m_nextState = SMManager::GetInstance()->GetSM(go->smID)->GetState("Path");

		switch (blueMinionSpawn[blueSpawnCount])
		{
		case 1:
			go = FetchGO(GameObject::GO_CREEP_MELEE);
			break;
		case 2:
			go = FetchGO(GameObject::GO_CREEP_RANGE);
			break;
		}
		blueSpawnCount = (blueSpawnCount + 1) % 6;
		go->nearest = NULL;
		go->faction = GameObject::FACTION_BLUE;
		go->currNode = blueBase;
		go->pos = blueBase->pos;
		AssignPath(go);
		//go->smID = "MinionSM";
		//go->m_currState = go->m_nextState = SMManager::GetInstance()->GetSM(go->smID)->GetState("Path");
	}

	ProjectileManager::GetInstance()->Update(dt * m_speed);
	static const float NPC_VELOCITY = 10.f;
	for (auto go : m_goList)
	{
		if (!go->active)
			continue;
		if (go->type == GameObject::GO_NPC)
		{
			if (go->target == NULL)
			{
				node* nearestNode = m_graph.NearestNode(go->pos);
				if (nearestNode != NULL)
				{
					go->target = nearestNode->pos;
					go->currNode = nearestNode;
				}
			}
			if (go->target != NULL)
			{
				if ((go->pos - go->target).LengthSquared() < 1.f)
				{
					go->pos = go->target;
					//if (!go->mapRead)
					{
						if (!go->mapRead)
						{
							go->gStack.push_back(go->currNode);
							go->m_visited.resize(m_graph.nodeID);
							std::fill(go->m_visited.begin(), go->m_visited.end(), false);
							//go->resize
							DFSOnce(go);
							go->target = go->currNode->pos;
							go->mapRead = true;
						}
						else if (!go->gStack.empty())
						{
							//go->resize
							DFSOnce(go);
							go->target = go->currNode->pos;
						}
						else if (!go->gPath.empty())
						{
							go->target = go->gPath.back();
							go->currNode = m_graph.NearestNode(go->target);
							go->gPath.pop_back();
						}
						else
						{
							//AStar
							if (clickTarget != NULL)
							{
								AStar(go, clickTarget);
							}
						}
					}
				}
				else
				{
					try
					{
						go->pos += (go->target - go->pos).Normalized() * NPC_VELOCITY * m_speed * static_cast<float>(dt);
					}
					catch (DivideByZero)
					{
						//Do nothing
					}
				}
			}
		}
		else if (go->type == GameObject::GO_CREEP_MELEE || go->type == GameObject::GO_CREEP_RANGE)
		{
			if (go->smID != "")
				SMManager::GetInstance()->GetSM(go->smID)->Update(dt * m_speed, go);		
		}
		else if (go->type == GameObject::GO_TOWER)
		{
			if (go->smID != "")
				SMManager::GetInstance()->GetSM(go->smID)->Update(dt * m_speed, go);
		}
	}
}


void SceneGraph::RenderGO(GameObject *go)
{
	static float healthBarScale = 0.7f;
	switch (go->type)
	{
	case GameObject::GO_NPC:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, 2);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_QUEEN], false);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_CREEP_MELEE:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, 2);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
			modelStack.PushMatrix();
			modelStack.Translate(0, go->scale.y * 0.1f, 0.5f);
			modelStack.Scale(healthBarScale, 0.1f, 1);
			RenderMesh(meshList[GEO_REDQUAD], false);
			modelStack.PopMatrix();
			modelStack.PushMatrix();
			modelStack.Translate(-0.5f * healthBarScale, go->scale.y * 0.1f, 1);
			modelStack.Translate(0.5f * go->health / go->maxhealth * healthBarScale, 0, 0);
			modelStack.Scale(healthBarScale * go->health / go->maxhealth, 0.1f, 1);
			RenderMesh(meshList[GEO_GREENQUAD], false);
			modelStack.PopMatrix();
		if(go->faction == GameObject::FACTION_BLUE)
			RenderMesh(meshList[GEO_BLUE_MELEE], false);
		else if (go->faction == GameObject::FACTION_RED)
			RenderMesh(meshList[GEO_RED_MELEE], false);
		else
			RenderMesh(meshList[GEO_BLUE_TRIANGLE], false);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_CREEP_RANGE:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, 2);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
			modelStack.PushMatrix();
			modelStack.Translate(0, go->scale.y * 0.1f, 0.5f);
			modelStack.Scale(healthBarScale, 0.1f, 1);
			RenderMesh(meshList[GEO_REDQUAD], false);
			modelStack.PopMatrix();
			modelStack.PushMatrix();
			modelStack.Translate(-0.5f * healthBarScale, go->scale.y * 0.1f, 1);
			modelStack.Translate(0.5f * go->health / go->maxhealth * healthBarScale, 0, 0);
			modelStack.Scale(healthBarScale * go->health / go->maxhealth, 0.1f, 1);
			RenderMesh(meshList[GEO_GREENQUAD], false);
			modelStack.PopMatrix();
		if (go->faction == GameObject::FACTION_BLUE)
			RenderMesh(meshList[GEO_BLUE_CASTER], false);
		else if (go->faction == GameObject::FACTION_RED)
			RenderMesh(meshList[GEO_RED_CASTER], false);
		else
			RenderMesh(meshList[GEO_BLUE_TRIANGLE], false);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_TOWER:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, 2);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		modelStack.PushMatrix();
		modelStack.Translate(0, go->scale.y * 0.1f, 0.5f);
		modelStack.Scale(healthBarScale, 0.1f, 1);
		RenderMesh(meshList[GEO_REDQUAD], false);
		modelStack.PopMatrix();
		modelStack.PushMatrix();
		modelStack.Translate(-0.5f * healthBarScale, go->scale.y * 0.1f, 1);
		modelStack.Translate(0.5f * go->health / go->maxhealth * healthBarScale, 0, 0);
		modelStack.Scale(healthBarScale * go->health / go->maxhealth, 0.1f, 1);
		RenderMesh(meshList[GEO_GREENQUAD], false);
		modelStack.PopMatrix();
		if (go->faction == GameObject::FACTION_BLUE)
			RenderMesh(meshList[GEO_BLUE_TOWER], false);
		else if (go->faction == GameObject::FACTION_RED)
			RenderMesh(meshList[GEO_RED_TOWER], false);
		else
			RenderMesh(meshList[GEO_BLUE_TRIANGLE], false);
		modelStack.PopMatrix();
		break;
	}
}

void SceneGraph::RenderGraph()
{
}

void SceneGraph::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Projection matrix : Orthographic Projection
	Mtx44 projection;
	projection.SetToOrtho(0, m_worldWidth, 0, m_worldHeight, -10, 10);
	projectionStack.LoadMatrix(projection);

	// Camera matrix
	viewStack.LoadIdentity();
	viewStack.LookAt(
		camera.position.x, camera.position.y, camera.position.z,
		camera.target.x, camera.target.y, camera.target.z,
		camera.up.x, camera.up.y, camera.up.z
		);
	// Model matrix : an identity matrix (model will be at the origin)
	modelStack.LoadIdentity();

	RenderMesh(meshList[GEO_AXES], false);

	modelStack.PushMatrix();
	modelStack.Translate(m_worldHeight * 0.5f, m_worldHeight * 0.5f, -1.f);
	modelStack.Scale(m_worldHeight, m_worldHeight, m_worldHeight);
	RenderMesh(meshList[GEO_BACKGROUND], false);
	modelStack.PopMatrix();

	//On screen text
	std::ostringstream ss;
	//Render the Graph
	for (auto edge : m_graph.m_edges)
	{
		modelStack.PushMatrix();
		modelStack.Translate(edge->src->pos.x, edge->src->pos.y, 0);
		Vector3 dir = edge->dst->pos - edge->src->pos;
			modelStack.PushMatrix();
			ss.str("");
			ss.precision(3);
			ss << edge->length;
			modelStack.Translate(dir.x * 0.5, dir.y * 0.5, 1);
			modelStack.Scale(3, 3, 1);
			//RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 2, edge->src->pos.x + dir.x * 0.5f, edge->src->pos.y + dir.y * 0.5f);
			RenderText(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0));
			modelStack.PopMatrix();
		modelStack.Rotate(Math::RadianToDegree(atan2(dir.y, dir.x)), 0, 0, 1);
		modelStack.Scale(edge->length, 1, 1);
		RenderMesh(meshList[GEO_LINE], false);
		modelStack.PopMatrix();	
	}
	for (auto node : m_graph.m_nodes)
	{
		modelStack.PushMatrix();
		modelStack.Translate(node->pos.x, node->pos.y, 0);
			modelStack.PushMatrix();
			ss.str("");
			ss.precision(3);
			ss << node->ID;
			modelStack.Translate(0, 3, 1);
			modelStack.Scale(3, 3, 1);
			//RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 2, edge->src->pos.x + dir.x * 0.5f, edge->src->pos.y + dir.y * 0.5f);
			RenderText(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0));
			modelStack.PopMatrix();		
		modelStack.Scale(4, 4, 1);
		modelStack.Rotate(90, 1, 0, 0);
		RenderMesh(meshList[GEO_GREYHEX], false);
		modelStack.PopMatrix();
	}

	for (auto go : m_goList)
	{
		if (!go->active)
			continue;
		RenderGO(go);
	}

	std::vector<Projectiles*>projList = ProjectileManager::GetInstance()->GetProjectileVector();
	for (auto pr : projList)
	{
		modelStack.PushMatrix();
		modelStack.Translate(pr->GetPosition().x, pr->GetPosition().y, 2);
		modelStack.Scale(pr->GetCollisionThreshhold(), pr->GetCollisionThreshhold(), pr->GetCollisionThreshhold());
		RenderMesh(meshList[GEO_RED_TRIANGLE], false);
		modelStack.PopMatrix();
	}

	ss.str("");
	switch (game_state)
	{
	case BLUE_WIN:
		ss << "Blue side Wins!";
		break;
	case RED_WIN:
		ss << "Red side Wins!";
		break;
	default:
		break;
	}
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 50, 21);

	ss.str("");
	ss << "Press 'R' to reset";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 50, 18);

	ss.str("");
	ss << "Press 'P' to change controller";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 50, 15);

	ss.str("");
	ss << "Player Controled: ";
	if(playerControlled)	
		ss << "true";
	else
		ss << "false";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 50, 12);

	if (playerControlled)
	{
		ss.str("");
		ss.precision(3);
		ss << "Current Lane: ";
		switch (currentSelected)
		{
		case BLUE_TOP:
			ss << "Top";
			break;
		case BLUE_MID:
			ss << "Middle";
			break;
		case BLUE_BOT:
			ss << "Bottom";
			break;
		default:
			ss << "Error";
			break;
		}
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 50, 9);
	}

	ss.str("");
	ss.precision(3);
	ss << "Speed:" << m_speed;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 50, 6);

	ss.str("");
	ss.precision(5);
	ss << "FPS:" << fps;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 50, 3);

	ss.str("");
	ss << "Graph " << 0;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 50, 0);
}

void SceneGraph::Exit()
{
	SceneBase::Exit();
	//Cleanup GameObjects
	while (m_goList.size() > 0)
	{
		GameObject *go = m_goList.back();
		delete go;
		m_goList.pop_back();
	}
}
