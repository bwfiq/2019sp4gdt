#include "SceneSP.h"
#include "GL\glew.h"
#include "Application.h"
#include <sstream>
#include "SceneData.h"

#include "SceneData.h"
#include "PostOffice.h"
#include "ConcreteMessages.h"
#include "ProjectileManager.h"

#include "SMManager.h"
#include "MouseController.h"

#include "Villager.h"
#include "Building.h"
#include "ChiefHut.h"

#define START_PLAYER false
#define SEA_WIDTH	50.f
#define SEA_HEIGHT	50.f
SceneSP::SceneSP()
{
}

SceneSP::~SceneSP()
{
}

void SceneSP::Init()
{
	SceneData::GetInstance()->SetNoGrid(10);
	SceneData::GetInstance()->SetGridSize(0.5f);
	SceneData::GetInstance()->SetGridOffset(0.25f);
	SceneBase::Init();

	m_grid.resize(SceneData::GetInstance()->GetNoGrid() * SceneData::GetInstance()->GetNoGrid());
	std::fill(m_grid.begin(), m_grid.end(), Grid::TILE_EMPTY);
	//Calculating aspect ratio
	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();
	SceneData::GetInstance()->SetWorldHeight(m_worldHeight);
	SceneData::GetInstance()->SetWorldWidth(m_worldWidth);
	SceneData::GetInstance()->SetElapsedTime(0);
	PostOffice::GetInstance()->Register("Scene", this);

	//Physics code here
	m_speed = 1.f;

	Math::InitRNG();

	goVillager = FetchGO(GameObject::GO_VILLAGER);
	goVillager->pos.Set(0, goVillager->scale.y * 0.5f, 0);

	goChiefHut = FetchGO(GameObject::GO_CHIEFHUT);
	goChiefHut->scale.y = 1.5f;
	goChiefHut->pos.Set(1.5f, goChiefHut->scale.y * 0.5f, 0);


	//go->vel.Set(1, 0, 0);
	MousePicker::GetInstance()->Init();
	MousePicker::GetInstance()->SetProjectionStack(projectionStack);
	MousePicker::GetInstance()->SetViewStack(viewStack);
}


bool SceneSP::Handle(Message* message)
{

	MessageWRU* messageWRU = dynamic_cast<MessageWRU*>(message);
	if (messageWRU)
	{
		switch (messageWRU->type)
		{
		case MessageWRU::PATH_TO_TARGET:
			AStarGrid(messageWRU->go, GetPoint(messageWRU->go->goTarget->pos));
			break;
		default:
			break;
		}
		delete message;
		return true;
	}
	delete message;
	return false;
}


GameObject* SceneSP::FetchGO(GameObject::GAMEOBJECT_TYPE type)
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
			case GameObject::GO_VILLAGER:
				go->scale.Set(1.f, 1.f, 1.f);
				break;
			}

			go->goTarget = NULL;
			return go;
		}
	}
	for (unsigned i = 0; i < 5; ++i)
	{
		GameObject *go = nullptr;
		switch (type)
		{
		case GameObject::GO_VILLAGER:
			go = new Villager(type);
			go->smID = "VillagerSM";
			go->m_currState = go->m_nextState = SMManager::GetInstance()->GetSM(go->smID)->GetState("Idle");
			break;
		case GameObject::GO_BUILDING:
			go = new Building(type);
			break;
		case GameObject::GO_CHIEFHUT:
			go = new ChiefHut(type);
			break;
		default:
			go = new GameObject(type);
			break;
		}
		if(go != nullptr)
			m_goList.push_back(go);
	}
	return FetchGO(type);
}
struct Compare2
{
	bool operator()(std::pair<GridPt, std::pair<int, int>> pair1, std::pair<GridPt, std::pair<int, int>> pair2)
	{
		return pair1.second.second < pair2.second.second;
	}
};

void SceneSP::AStarGrid(GameObject * go, GridPt target)
{

	//first = curr point, second.first = tile cost, second.second = total cost(tile cost + distance to final destination)
	std::vector<std::pair<GridPt, std::pair<int, int>>>priority_Queue;
	std::sort(priority_Queue.begin(), priority_Queue.end(), Compare2());
	while (!m_shortestPath.empty())
	{
		m_shortestPath.pop_back();
	}
	SceneData* SD = SceneData::GetInstance();
	go->m_visited.resize(SD->GetNoGrid() * SD->GetNoGrid());
	std::fill(go->m_visited.begin(), go->m_visited.end(), false);
	//keeping track of the parent node
	m_previous.resize(SD->GetNoGrid() * SD->GetNoGrid());
	std::fill(m_previous.begin(), m_previous.end(), NULL);

	GridPt curr = GetPoint(go->pos);
	if (!isPointInGrid(curr))
		return;

	GridPt bestStart = curr; //Closest gridPt to start from
	float fDistanceToTarget = (target.x - curr.x) * (target.x - curr.x) + (target.z - curr.z) * (target.z - curr.z);
	Vector3 ptPos = GetGridPos(curr); //Position of gridPt in world space

	//Check if goPos is left of or right of gridPt
	if (go->pos.x < ptPos.x)
	{
		//Check left of gridPt
		GridPt temp = GridPt(curr.x - 1, curr.z);
		if (isPointInGrid(temp))
		{
			float distance = (target.x - temp.x) * (target.x - temp.x) + (target.z - temp.z) * (target.z - temp.z);
			if (distance < fDistanceToTarget)
			{
				fDistanceToTarget = distance;
				bestStart = temp;
			}
		}
		//Check if goPos is above or below center of gridPt
		if (go->pos.z < ptPos.z)
		{
			//Check above of gridPt
			temp = GridPt(curr.x, curr.z - 1);
			if (isPointInGrid(temp))
			{
				float distance = (target.x - temp.x) * (target.x - temp.x) + (target.z - temp.z) * (target.z - temp.z);
				if (distance < fDistanceToTarget)
				{
					fDistanceToTarget = distance;
					bestStart = temp;
				}
			}
			//Check diagonal
			temp = GridPt(curr.x - 1, curr.z - 1);
			if (isPointInGrid(temp))
			{
				float distance = (target.x - temp.x) * (target.x - temp.x) + (target.z - temp.z) * (target.z - temp.z);
				if (distance < fDistanceToTarget)
				{
					fDistanceToTarget = distance;
					bestStart = temp;
				}
			}
		}
		else
		{
			//Check below of gridPt
			temp = GridPt(curr.x, curr.z + 1);
			if (isPointInGrid(temp))
			{
				float distance = (target.x - temp.x) * (target.x - temp.x) + (target.z - temp.z) * (target.z - temp.z);
				if (distance < fDistanceToTarget)
				{
					fDistanceToTarget = distance;
					bestStart = temp;
				}
			}
			//Check diagonal
			temp = GridPt(curr.x - 1, curr.z + 1);
			if (isPointInGrid(temp))
			{
				float distance = (target.x - temp.x) * (target.x - temp.x) + (target.z - temp.z) * (target.z - temp.z);
				if (distance < fDistanceToTarget)
				{
					fDistanceToTarget = distance;
					bestStart = temp;
				}
			}
		}
	}
	else
	{
		//Check right of gridPt
		GridPt temp = GridPt(curr.x + 1, curr.z);
		if (isPointInGrid(temp))
		{
			float distance = (target.x - temp.x) * (target.x - temp.x) + (target.z - temp.z) * (target.z - temp.z);
			if (distance < fDistanceToTarget)
			{
				fDistanceToTarget = distance;
				bestStart = temp;
			}
		}
		//Check if goPos is above or below center of gridPt
		if (go->pos.z < ptPos.z)
		{
			//Check above of gridPt
			temp = GridPt(curr.x, curr.z - 1);
			if (isPointInGrid(temp))
			{
				float distance = (target.x - temp.x) * (target.x - temp.x) + (target.z - temp.z) * (target.z - temp.z);
				if (distance < fDistanceToTarget)
				{
					fDistanceToTarget = distance;
					bestStart = temp;
				}
			}
			//Check diagonal
			temp = GridPt(curr.x + 1, curr.z - 1);
			if (isPointInGrid(temp))
			{
				float distance = (target.x - temp.x) * (target.x - temp.x) + (target.z - temp.z) * (target.z - temp.z);
				if (distance < fDistanceToTarget)
				{
					fDistanceToTarget = distance;
					bestStart = temp;
				}
			}
		}
		else
		{
			//Check below of gridPt
			temp = GridPt(curr.x, curr.z + 1);
			if (isPointInGrid(temp))
			{
				float distance = (target.x - temp.x) * (target.x - temp.x) + (target.z - temp.z) * (target.z - temp.z);
				if (distance < fDistanceToTarget)
				{
					fDistanceToTarget = distance;
					bestStart = temp;
				}
			}
			//Check diagonal
			temp = GridPt(curr.x + 1, curr.z + 1);
			if (isPointInGrid(temp))
			{
				float distance = (target.x - temp.x) * (target.x - temp.x) + (target.z - temp.z) * (target.z - temp.z);
				if (distance < fDistanceToTarget)
				{
					fDistanceToTarget = distance;
					bestStart = temp;
				}
			}
		}
	}

	int G = 0;
	priority_Queue.push_back(std::pair<GridPt, std::pair<int, int>>(bestStart, std::pair<int, int>(G, 0)));
	go->m_visited[GetGridIndex(bestStart)] = true;
	float nearestDistance = FLT_MAX;
	GridPt nearestTile = bestStart;
	GridPt next = bestStart;
	int currIndex = 0;

	for (int loop = 0; loop < SD->GetNoGrid() * SD->GetNoGrid() && !priority_Queue.empty(); ++loop)
	{
		//std::cout << "One Round of Loop" << std::endl;
		curr = priority_Queue.begin()->first;
		//m_queue.pop();

		if (curr == target)
		{
			break;
		}

		//Check Up
		G = priority_Queue.begin()->second.first;
		next.Set(curr.x, curr.z - 1);
		currIndex = GetGridIndex(next);
		if (isPointInGrid(next))
		{
			float distanceSquared = NULL;
			if (m_grid[currIndex] == Grid::TILE_EMPTY)
			{
				//Cost of Empty Tile
				G += 1;
				//Calculate distance to end
				distanceSquared = (int)Math::FAbs(target.x - next.x) + (int)Math::FAbs(target.z - next.z);
			}
			if (!go->m_visited[currIndex])
			{
				//Update node cost
				m_previous[currIndex] = curr;
				priority_Queue.push_back(std::pair<GridPt, std::pair<int, int>>(next, std::pair<int, int>(G, G + distanceSquared)));
				go->m_visited[currIndex] = true;

				//Check if it is the nearest Tile
				if (distanceSquared < nearestDistance)
				{
					nearestDistance = distanceSquared;
					nearestTile = next;
				}
			}
			else if (distanceSquared != NULL)
			{
				//Checks if visited node is in queue
				for (std::vector<std::pair<GridPt, std::pair<int, int>>>::iterator it = priority_Queue.begin(); it != priority_Queue.end(); ++it)
				{
					std::pair<GridPt, std::pair<int, int>> path = *it;
					if (path.first == next)
					{
						//If in queue, check if new path is shorter thn old path
						if (path.second.second > G + distanceSquared)
						{
							//Update if new path is shorter
							priority_Queue.erase(it);
							priority_Queue.push_back(std::pair<GridPt, std::pair<int, int>>(next, std::pair<int, int>(G, G + distanceSquared)));
							m_previous[currIndex] = curr;
						}
						break;
					}
				}
				//If not in queue, node is already in optimal path
			}
		}

		//Check Down
		G = priority_Queue.begin()->second.first;
		next.Set(curr.x, curr.z + 1);
		currIndex = GetGridIndex(next);
		if (isPointInGrid(next))
		{
			float distanceSquared = NULL;
			if (m_grid[currIndex] == Grid::TILE_EMPTY)
			{
				//Cost of Empty Tile
				G += 1;
				//Calculate distance to end
				distanceSquared = (int)Math::FAbs(target.x - next.x) + (int)Math::FAbs(target.z - next.z);
			}
			if (!go->m_visited[currIndex])
			{
				//Update node cost
				m_previous[currIndex] = curr;
				priority_Queue.push_back(std::pair<GridPt, std::pair<int, int>>(next, std::pair<int, int>(G, G + distanceSquared)));
				go->m_visited[currIndex] = true;

				//Check if it is the nearest Tile
				if (distanceSquared < nearestDistance)
				{
					nearestDistance = distanceSquared;
					nearestTile = next;
				}
			}
			else if (distanceSquared != NULL)
			{
				//Checks if visited node is in queue
				for (std::vector<std::pair<GridPt, std::pair<int, int>>>::iterator it = priority_Queue.begin(); it != priority_Queue.end(); ++it)
				{
					std::pair<GridPt, std::pair<int, int>> path = *it;
					if (path.first == next)
					{
						//If in queue, check if new path is shorter thn old path
						if (path.second.second > G + distanceSquared)
						{
							//Update if new path is shorter
							priority_Queue.erase(it);
							priority_Queue.push_back(std::pair<GridPt, std::pair<int, int>>(next, std::pair<int, int>(G, G + distanceSquared)));
							m_previous[currIndex] = curr;
						}
						break;
					}
				}
				//If not in queue, node is already in optimal path
			}
		}

		//Check Left
		G = priority_Queue.begin()->second.first;
		next.Set(curr.x - 1, curr.z);
		currIndex = GetGridIndex(next);
		if (isPointInGrid(next))
		{
			float distanceSquared = NULL;
			if (m_grid[currIndex] == Grid::TILE_EMPTY)
			{
				//Cost of Empty Tile
				G += 1;
				//Calculate distance to end
				distanceSquared = (int)Math::FAbs(target.x - next.x) + (int)Math::FAbs(target.z - next.z);
			}
			if (!go->m_visited[currIndex])
			{
				//Update node cost
				m_previous[currIndex] = curr;
				priority_Queue.push_back(std::pair<GridPt, std::pair<int, int>>(next, std::pair<int, int>(G, G + distanceSquared)));
				go->m_visited[currIndex] = true;

				//Check if it is the nearest Tile
				if (distanceSquared < nearestDistance)
				{
					nearestDistance = distanceSquared;
					nearestTile = next;
				}
			}
			else if (distanceSquared != NULL)
			{
				//Checks if visited node is in queue
				for (std::vector<std::pair<GridPt, std::pair<int, int>>>::iterator it = priority_Queue.begin(); it != priority_Queue.end(); ++it)
				{
					std::pair<GridPt, std::pair<int, int>> path = *it;
					if (path.first == next)
					{
						//If in queue, check if new path is shorter thn old path
						if (path.second.second > G + distanceSquared)
						{
							//Update if new path is shorter
							priority_Queue.erase(it);
							priority_Queue.push_back(std::pair<GridPt, std::pair<int, int>>(next, std::pair<int, int>(G, G + distanceSquared)));
							m_previous[currIndex] = curr;
						}
						break;
					}
				}
				//If not in queue, node is already in optimal path
			}
		}

		//Check Right
		G = priority_Queue.begin()->second.first;
		next.Set(curr.x + 1, curr.z);
		currIndex = GetGridIndex(next);
		if (isPointInGrid(next))
		{
			float distanceSquared = NULL;
			if (m_grid[currIndex] == Grid::TILE_EMPTY)
			{
				//Cost of Empty Tile
				G += 1;
				//Calculate distance to end
				distanceSquared = (int)Math::FAbs(target.x - next.x) + (int)Math::FAbs(target.z - next.z);
			}
			if (!go->m_visited[currIndex])
			{
				//Update node cost
				m_previous[currIndex] = curr;
				priority_Queue.push_back(std::pair<GridPt, std::pair<int, int>>(next, std::pair<int, int>(G, G + distanceSquared)));
				go->m_visited[currIndex] = true;

				//Check if it is the nearest Tile
				if (distanceSquared < nearestDistance)
				{
					nearestDistance = distanceSquared;
					nearestTile = next;
				}
			}
			else if (distanceSquared != NULL)
			{
				//Checks if visited node is in queue
				for (std::vector<std::pair<GridPt, std::pair<int, int>>>::iterator it = priority_Queue.begin(); it != priority_Queue.end(); ++it)
				{
					std::pair<GridPt, std::pair<int, int>> path = *it;
					if (path.first == next)
					{
						//If in queue, check if new path is shorter thn old path
						if (path.second.second > G + distanceSquared)
						{
							//Update if new path is shorter
							priority_Queue.erase(it);
							priority_Queue.push_back(std::pair<GridPt, std::pair<int, int>>(next, std::pair<int, int>(G, G + distanceSquared)));
							m_previous[currIndex] = curr;
						}
						break;
					}
				}
				//If not in queue, node is already in optimal path
			}
		}

		//Check Up Left
		G = priority_Queue.begin()->second.first;
		next.Set(curr.x - 1, curr.z - 1);
		currIndex = GetGridIndex(next);
		if (isPointInGrid(next))
		{
			float distanceSquared = NULL;
			if (m_grid[currIndex] == Grid::TILE_EMPTY)
			{
				//Cost of Empty Tile
				G += 1;
				//Calculate distance to end
				distanceSquared = (int)Math::FAbs(target.x - next.x) + (int)Math::FAbs(target.z - next.z);
			}
			if (!go->m_visited[currIndex])
			{
				//Update node cost
				m_previous[currIndex] = curr;
				priority_Queue.push_back(std::pair<GridPt, std::pair<int, int>>(next, std::pair<int, int>(G, G + distanceSquared)));
				go->m_visited[currIndex] = true;

				//Check if it is the nearest Tile
				if (distanceSquared < nearestDistance)
				{
					nearestDistance = distanceSquared;
					nearestTile = next;
				}
			}
			else if (distanceSquared != NULL)
			{
				//Checks if visited node is in queue
				for (std::vector<std::pair<GridPt, std::pair<int, int>>>::iterator it = priority_Queue.begin(); it != priority_Queue.end(); ++it)
				{
					std::pair<GridPt, std::pair<int, int>> path = *it;
					if (path.first == next)
					{
						//If in queue, check if new path is shorter thn old path
						if (path.second.second > G + distanceSquared)
						{
							//Update if new path is shorter
							priority_Queue.erase(it);
							priority_Queue.push_back(std::pair<GridPt, std::pair<int, int>>(next, std::pair<int, int>(G, G + distanceSquared)));
							m_previous[currIndex] = curr;
						}
						break;
					}
				}
				//If not in queue, node is already in optimal path
			}
		}

		//Check Up Right
		G = priority_Queue.begin()->second.first;
		next.Set(curr.x + 1, curr.z - 1);
		currIndex = GetGridIndex(next);
		if (isPointInGrid(next))
		{
			float distanceSquared = NULL;
			if (m_grid[currIndex] == Grid::TILE_EMPTY)
			{
				//Cost of Empty Tile
				G += 1;
				//Calculate distance to end
				distanceSquared = (int)Math::FAbs(target.x - next.x) + (int)Math::FAbs(target.z - next.z);
			}
			if (!go->m_visited[currIndex])
			{
				//Update node cost
				m_previous[currIndex] = curr;
				priority_Queue.push_back(std::pair<GridPt, std::pair<int, int>>(next, std::pair<int, int>(G, G + distanceSquared)));
				go->m_visited[currIndex] = true;

				//Check if it is the nearest Tile
				if (distanceSquared < nearestDistance)
				{
					nearestDistance = distanceSquared;
					nearestTile = next;
				}
			}
			else if (distanceSquared != NULL)
			{
				//Checks if visited node is in queue
				for (std::vector<std::pair<GridPt, std::pair<int, int>>>::iterator it = priority_Queue.begin(); it != priority_Queue.end(); ++it)
				{
					std::pair<GridPt, std::pair<int, int>> path = *it;
					if (path.first == next)
					{
						//If in queue, check if new path is shorter thn old path
						if (path.second.second > G + distanceSquared)
						{
							//Update if new path is shorter
							priority_Queue.erase(it);
							priority_Queue.push_back(std::pair<GridPt, std::pair<int, int>>(next, std::pair<int, int>(G, G + distanceSquared)));
							m_previous[currIndex] = curr;
						}
						break;
					}
				}
				//If not in queue, node is already in optimal path
			}
		}

		//Check Down Left
		G = priority_Queue.begin()->second.first;
		next.Set(curr.x - 1, curr.z + 1);
		currIndex = GetGridIndex(next);
		if (isPointInGrid(next))
		{
			float distanceSquared = NULL;
			if (m_grid[currIndex] == Grid::TILE_EMPTY)
			{
				//Cost of Empty Tile
				G += 1;
				//Calculate distance to end
				distanceSquared = (int)Math::FAbs(target.x - next.x) + (int)Math::FAbs(target.z - next.z);
			}
			if (!go->m_visited[currIndex])
			{
				//Update node cost
				m_previous[currIndex] = curr;
				priority_Queue.push_back(std::pair<GridPt, std::pair<int, int>>(next, std::pair<int, int>(G, G + distanceSquared)));
				go->m_visited[currIndex] = true;

				//Check if it is the nearest Tile
				if (distanceSquared < nearestDistance)
				{
					nearestDistance = distanceSquared;
					nearestTile = next;
				}
			}
			else if (distanceSquared != NULL)
			{
				//Checks if visited node is in queue
				for (std::vector<std::pair<GridPt, std::pair<int, int>>>::iterator it = priority_Queue.begin(); it != priority_Queue.end(); ++it)
				{
					std::pair<GridPt, std::pair<int, int>> path = *it;
					if (path.first == next)
					{
						//If in queue, check if new path is shorter thn old path
						if (path.second.second > G + distanceSquared)
						{
							//Update if new path is shorter
							priority_Queue.erase(it);
							priority_Queue.push_back(std::pair<GridPt, std::pair<int, int>>(next, std::pair<int, int>(G, G + distanceSquared)));
							m_previous[currIndex] = curr;
						}
						break;
					}
				}
				//If not in queue, node is already in optimal path
			}
		}

		//Check Down Right
		G = priority_Queue.begin()->second.first;
		next.Set(curr.x + 1, curr.z + 1);
		currIndex = GetGridIndex(next);
		if (isPointInGrid(next))
		{
			float distanceSquared = NULL;
			if (m_grid[currIndex] == Grid::TILE_EMPTY)
			{
				//Cost of Empty Tile
				G += 1;
				//Calculate distance to end
				distanceSquared = (int)Math::FAbs(target.x - next.x) + (int)Math::FAbs(target.z - next.z);
			}
			if (!go->m_visited[currIndex])
			{
				//Update node cost
				m_previous[currIndex] = curr;
				priority_Queue.push_back(std::pair<GridPt, std::pair<int, int>>(next, std::pair<int, int>(G, G + distanceSquared)));
				go->m_visited[currIndex] = true;

				//Check if it is the nearest Tile
				if (distanceSquared < nearestDistance)
				{
					nearestDistance = distanceSquared;
					nearestTile = next;
				}
			}
			else if (distanceSquared != NULL)
			{
				//Checks if visited node is in queue
				for (std::vector<std::pair<GridPt, std::pair<int, int>>>::iterator it = priority_Queue.begin(); it != priority_Queue.end(); ++it)
				{
					std::pair<GridPt, std::pair<int, int>> path = *it;
					if (path.first == next)
					{
						//If in queue, check if new path is shorter thn old path
						if (path.second.second > G + distanceSquared)
						{
							//Update if new path is shorter
							priority_Queue.erase(it);
							priority_Queue.push_back(std::pair<GridPt, std::pair<int, int>>(next, std::pair<int, int>(G, G + distanceSquared)));
							m_previous[currIndex] = curr;
						}
						break;
					}
				}
				//If not in queue, node is already in optimal path
			}
		}

		priority_Queue.erase(priority_Queue.begin());
		std::sort(priority_Queue.begin(), priority_Queue.end(), Compare2());
	}

	if (priority_Queue.begin()->first == target)
	{
		//If manage to reach target
		curr = target;
		while (curr != NULL)
		{
			m_shortestPath.push_back(curr);
			curr = m_previous[GetGridIndex(curr)];
		}
	}
	else
	{
		//If unable to reach target
		curr = nearestTile;
		while (curr != NULL)
		{
			m_shortestPath.push_back(curr);
			curr = m_previous[GetGridIndex(curr)];
		}
	}

	//Just take based on range of movement 
	while (!m_shortestPath.empty())
	{
		go->path.push_back(m_shortestPath.back());
		m_shortestPath.pop_back();
	}
	if (go->path.size() > 1)
	{
		std::reverse(go->path.begin(), go->path.end());
	}
	//std::cout << priority_Queue.begin()->second.first << std::endl;
	//return priority_Queue.begin()->second.first;
}

//void SceneSP::AStar(GameObject * go, Vector3 target)
//{
//	go->m_visited.resize(m_graph.nodeID);
//	std::fill(go->m_visited.begin(), go->m_visited.end(), false);
//	go->gPath.clear();
//	node* nearestToTarget = m_graph.NearestNode(target);
//
//	//node* is the current node, first float is cost to node, second float is cost to node + distance to end
//	std::vector<std::pair<node*, std::pair<float, float>>> m_queue;
//	m_queue.push_back(std::pair<node*, std::pair<float, float>>(go->currNode, std::pair<float, float>(0.f, (nearestToTarget->pos - go->currNode->pos).LengthSquared())));
//	while (m_queue.front().first != nearestToTarget && !m_queue.empty())
//	{
//
//		std::pair<node*, std::pair<float, float>> curr = m_queue.front();
//		node* currNode = curr.first;
//
//		for (auto edges : currNode->edges)
//		{
//			float cost = curr.second.first + edges->length;
//			float costToEnd = cost + (nearestToTarget->pos - edges->dst->pos).LengthSquared();
//			std::pair<node*, std::pair<float, float>> value(edges->dst, std::pair<float, float>(cost, costToEnd));
//			if (!go->m_visited[edges->dst->ID - 1])
//			{
//				edges->dst->previous = currNode;
//				go->m_visited[edges->dst->ID - 1] = true;
//				bool inserted = false;
//				for (std::vector<std::pair<node*, std::pair<float, float>>>::iterator it = m_queue.begin(); it != m_queue.end(); ++it)
//				{
//					std::pair<node*, std::pair<float, float>> temp = *it;
//					if (value.second.second < temp.second.second)
//					{
//						m_queue.insert(it,value);
//						inserted = true;
//						break;
//					}
//				}
//				if (!inserted)
//				{
//					m_queue.push_back(value);
//				}
//			}
//			else
//			{
//				for (auto go : m_queue)
//				{
//					if (go.first == value.first)
//					{
//						edges->dst->previous = currNode;
//						go = value;
//						break;
//					}
//				}
//			}
//		}
//		std::vector<std::pair<node*, std::pair<float, float>>>::iterator tempit;
//		tempit = std::find(m_queue.begin(), m_queue.end(), curr);
//		m_queue.erase(tempit);
//	}
//
//	node* curr = nearestToTarget;
//	while (curr != go->currNode)
//	{
//		go->gPath.push_back(curr->pos);
//		curr = curr->previous;
//	}
//}
//
//void SceneSP::DFSOnce(GameObject * go)
//{
//	go->gStack.push_back(go->currNode);
//	go->m_visited[go->currNode->ID - 1] = true;
//	for (auto edge : go->currNode->edges)
//	{
//		if (!go->m_visited[edge->dst->ID - 1])
//		{
//			go->currNode = edge->dst;
//			return;
//		}
//	}
//	go->gStack.pop_back();
//	if (!go->gStack.empty())
//	{
//		go->currNode = go->gStack.back();
//		go->gStack.pop_back();
//	}
//	return;
//}

void SceneSP::Reset()
{	
	//Cleanup GameObjects
	while (m_goList.size() > 0)
	{
		GameObject *go = m_goList.back();
		delete go;
		m_goList.pop_back();
	}

	m_speed = 1.f;

	//game_state = NEUTRAL;
}

void SceneSP::Update(double dt)
{
	if (Application::IsKeyPressed('R'))
	{
		Reset();
		Sleep(100);
		return;
	}
	SceneData* SD = SceneData::GetInstance();
	MousePicker* MP = MousePicker::GetInstance();
	MouseController* MC = MouseController::GetInstance();

	SceneBase::Update(dt);
	MP->Update(dt);
	camera.Update(dt);

	//Calculating aspect ratio
	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();

	

	SD->SetWorldHeight(m_worldHeight);
	SD->SetWorldWidth(m_worldWidth);
	SD->SetElapsedTime(SD->GetElapsedTime() + (float)dt);

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
	if (MC->IsButtonPressed(MouseController::LMB))
		std::cout << "asd" << std::endl;

	//Input Section
	static bool bPState = false;
	if (!bPState && Application::IsKeyPressed('P'))
	{
		bPState = true;
	}
	else if(bPState && !Application::IsKeyPressed('P'))
	{
		bPState = false;
		std::cout << "P UP" << std::endl;
	}

	static bool bFState = false;
	if (!bFState && Application::IsKeyPressed('F'))
	{
		bFState = true;
		goVillager->goTarget = goChiefHut;
	}
	else if (bFState && !Application::IsKeyPressed('F'))
	{
		bFState = false;
		std::cout << "F UP" << std::endl;
	}

	static bool bGState = false;
	if (!bGState && Application::IsKeyPressed('G'))
	{
		bGState = true;
		switch (m_grid[50])
		{
		case Grid::TILE_EMPTY:
			m_grid[50] = Grid::TILE_USED;
			break;
		case Grid::TILE_USED:
			m_grid[50] = Grid::TILE_EMPTY;
			break;
		}
	}
	else if (bGState && !Application::IsKeyPressed('G'))
	{
		bGState = false;
		std::cout << "G UP" << std::endl;
	}

	//Temporary Movement
	GridPt currPos = GetPoint(goVillager->pos);
	static bool bWState = false;
	if (!bWState && Application::IsKeyPressed('W'))
	{
		bWState = true;
		if (goVillager->m_currState == SMManager::GetInstance()->GetSM(goVillager->smID)->GetState("Idle"))
		{
			GridPt UP(currPos.x, currPos.z - 1);
			if (isPointInGrid(UP))
			{
				float y = goVillager->pos.y;
				goVillager->target = GetGridPos(UP);
				goVillager->target.y = y;
			}
		}
	}
	else if (bWState && !Application::IsKeyPressed('W'))
	{
		bWState = false;
		std::cout << "W UP" << std::endl;
	}
	static bool bSState = false;
	if (!bSState && Application::IsKeyPressed('S'))
	{
		bSState = true;
		if (goVillager->m_currState == SMManager::GetInstance()->GetSM(goVillager->smID)->GetState("Idle"))
		{
			GridPt DOWN(currPos.x, currPos.z + 1);
			if (isPointInGrid(DOWN))
			{
				float y = goVillager->pos.y;
				goVillager->target = GetGridPos(DOWN);
				goVillager->target.y = y;
			}
		}
	}
	else if (bSState && !Application::IsKeyPressed('S'))
	{
		bSState = false;
		std::cout << "S UP" << std::endl;
	}
	static bool bAState = false;
	if (!bAState && Application::IsKeyPressed('A'))
	{
		bAState = true;
		if (goVillager->m_currState == SMManager::GetInstance()->GetSM(goVillager->smID)->GetState("Idle"))
		{
			GridPt LEFT(currPos.x - 1, currPos.z);
			if (isPointInGrid(LEFT))
			{
				float y = goVillager->pos.y;
				goVillager->target = GetGridPos(LEFT);
				goVillager->target.y = y;
			}
		}
	}
	else if (bAState && !Application::IsKeyPressed('A'))
	{
		bAState = false;
		std::cout << "A UP" << std::endl;
	}
	static bool bDState = false;
	if (!bDState && Application::IsKeyPressed('D'))
	{
		bDState = true;
		if (goVillager->m_currState == SMManager::GetInstance()->GetSM(goVillager->smID)->GetState("Idle"))
		{
			GridPt RIGHT(currPos.x + 1, currPos.z);
			if (isPointInGrid(RIGHT))
			{
				float y = goVillager->pos.y;
				goVillager->target = GetGridPos(RIGHT);
				goVillager->target.y = y;
			}
		}
	}
	else if (bDState && !Application::IsKeyPressed('D'))
	{
		bDState = false;
		std::cout << "D UP" << std::endl;
	}

	/*
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
	*/
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
			//GameObject* go = FetchGO(GameObject::GO_NPC);
			//go->pos.Set(posX, posY);
		}
	}
	else if (bSpaceState && !Application::IsKeyPressed(VK_SPACE))
	{
		bSpaceState = false;
	}

	// sea movement
	/*if (Application::IsKeyPressed('I'))
		fSeaDeltaZ += 0.5f * dt;
	if (Application::IsKeyPressed('K'))
		fSeaDeltaZ -= 0.5f * dt;
	if (Application::IsKeyPressed('J'))
		fSeaDeltaX += 0.5f * dt;
	if (Application::IsKeyPressed('L'))
		fSeaDeltaX -= 0.5f * dt;


	fSeaDeltaX += (5.f - fSeaDeltaX) *0.1f * dt;*/
	if (fSeaDeltaX >= SEA_WIDTH / 4)
		fSeaDeltaX = -SEA_WIDTH / 4;
	fSeaDeltaX += dt;
	if (fSeaDeltaZ >= SEA_HEIGHT / 4)
		fSeaDeltaZ = -SEA_HEIGHT / 4;
	fSeaDeltaZ += dt;

	fSeaDeltaY = 0.25f + 0.25f * cosf(SD->GetElapsedTime());

	ProjectileManager::GetInstance()->Update(dt * m_speed);
	static const float NPC_VELOCITY = 10.f;
	for (auto go : m_goList)
	{
		if (!go->active)
			continue;
		/*if (go->type == GameObject::GO_NPC)
		{
			if (go->target == NULL)
			{
			}
			if (go->target != NULL)
			{
				if ((go->pos - go->target).LengthSquared() < 1.f)
				{
					go->pos = go->target;
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
			{
				SMManager::GetInstance()->GetSM(go->smID)->Update(dt * m_speed, go);
			}
		}*/
		//go->pos += go->vel * dt * m_speed;
		if (go->smID != "")
		{
			SMManager::GetInstance()->GetSM(go->smID)->Update(dt * m_speed, go);
		}
	}
}


void SceneSP::RenderGO(GameObject *go)
{
	static float healthBarScale = 0.7f;
	switch (go->type)
	{
	case GameObject::GO_VILLAGER:
	{
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_VILLAGER], false, 1.f);
		GridPt temp = GetPoint(go->pos);
		std::cout << temp.x << " " << temp.z << std::endl;
		modelStack.PopMatrix();
	}
	break; 
	case GameObject::GO_BUILDING:
	{	
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_BUILDING], false, 1.f);
		modelStack.PopMatrix();
	}
		break;
	case GameObject::GO_CHIEFHUT:
	{	
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_BUILDING], false, 1.f);
		modelStack.PopMatrix();
	}
		break;
	default:
		break;
	}
}

void SceneSP::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Projection matrix : Orthographic Projection
	Mtx44 projection;
	//projection.SetToOrtho(0, m_worldWidth, 0, m_worldHeight, -1000, 1000);
	float aspect = (float)Application::GetInstance().GetWindowWidth() / (float)Application::GetInstance().GetWindowHeight();
	projection.SetToPerspective(70.f, aspect, 0.01f, 2000);
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

	modelStack.PushMatrix();
	Vector3 adsada = MousePicker::GetInstance()->GetIntersectionWithPlane(camera.position, Vector3(0, 0.6f, 0), Vector3(0, 1, 0));
	modelStack.Translate(adsada.x, adsada.y, adsada.z);
	modelStack.Scale(0.025, 0.025, 0.025);
	RenderMesh(meshList[GEO_VILLAGER], false);
	modelStack.PopMatrix();

	RenderMesh(meshList[GEO_AXES], false);

	static float asd = 0;
	asd += 0.01;
	modelStack.PushMatrix();
	//modelStack.Translate(0, 0.5f + cosf(asd) * 0.15f, 0);
	modelStack.Translate(0, -0.5f, 0);
	//modelStack.Rotate(-90, 1, 0, 0);
	modelStack.Scale(10, 1, 10);
	RenderMesh(meshList[GEO_GRASS], false);
	modelStack.PopMatrix();


	SceneData* SD = SceneData::GetInstance();
	modelStack.PushMatrix();
	modelStack.Translate(-0.5f * SD->GetNoGrid() * SD->GetGridSize(), 0.001f, -0.5f * SD->GetNoGrid() * SD->GetGridSize());
	modelStack.Scale(1, 1, 1);
	glLineWidth(2.f);
	RenderMesh(meshList[GEO_GRID], false);
	glLineWidth(1.f);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-0.5f * SD->GetNoGrid() * SD->GetGridSize(), 0.001f, -0.5f * SD->GetNoGrid() * SD->GetGridSize());
	for (int i = 0; i < SD->GetNoGrid() * SD->GetNoGrid(); ++i)
	{
		std::pair<int, int> pt = GetPoint(i);
		modelStack.PushMatrix();
		modelStack.Translate(pt.first * SD->GetGridSize() + SD->GetGridOffset(), 0, pt.second * SD->GetGridSize() + SD->GetGridOffset());
		modelStack.Rotate(-90, 1, 0, 0);
		modelStack.Scale(SD->GetGridSize(), SD->GetGridSize(), SD->GetGridSize());
		switch (m_grid[i])
		{
		case Grid::TILE_EMPTY:
			RenderMesh(meshList[GEO_GREENQUAD], false, 0.3f);
			break;
		case Grid::TILE_USED:
			RenderMesh(meshList[GEO_REDQUAD], false, 0.3f);
			break;
		}
		modelStack.PopMatrix();
	}
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(fSeaDeltaX, fSeaDeltaY - 0.51f, fSeaDeltaZ);
	modelStack.Rotate(-90, 1, 0, 0);
	modelStack.Scale(SEA_WIDTH, 50, 50);
	RenderMesh(meshList[GEO_SEA], false);
	modelStack.PopMatrix();

	//On screen text
	std::ostringstream ss;

	for (auto go : m_goList)
	{
		if (!go->active)
			continue;
		RenderGO(go);
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

void SceneSP::Exit()
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
