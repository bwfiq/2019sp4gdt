#include "SceneSP.h"
#include "GL\glew.h"
#include "Application.h"
#include <sstream>
#include "SceneData.h"

#include "SceneData.h"
#include "PostOffice.h"
#include "ConcreteMessages.h"
#include "ProjectileManager.h"
#include "UIManager.h"
#include "UIReligionBar.h"
#include "UIMenuButton.h"
#include "EffectManager.h"
#include "EffectTrail.h"
#include "EffectHand.h"

#include "CalamityManager.h"
#include "CalamityEarthquake.h"

#include "SMManager.h"
#include "MouseController.h"
#include "KeyboardController.h"

#include "Villager.h"
#include "Building.h"
#include "ChiefHut.h"
#include "Bush.h"
#include "Tree.h"
#include "Altar.h"

#define SEA_WIDTH	100.f
#define SEA_HEIGHT	100.f

SceneSP::SceneSP()
{
}

SceneSP::~SceneSP()
{
}

float FoodToReligionPoints(int food)
{
	//Insert Conversion Rates
	return (float)(food) * 1;//* some dt related converter or someth
}

bool SceneSP::isTheCoastClear(GameObject* go, GridPt next, Grid::DIRECTION dir)
{
	if (go->iGridX > 0 && go->iGridZ > 0)
	{
		int iHalfGridX = go->iGridX * 0.5f;
		int iHalfGridZ = go->iGridZ * 0.5f;
		GridPt ptAhead;
		switch (dir)
		{
		case Grid::DIR_LEFT:
			for (int zAxis = 1; zAxis <= iHalfGridZ; ++zAxis)
			{
				ptAhead.Set(next.x - iHalfGridX, next.z + zAxis);
				if (isPointInGrid(ptAhead))
				{
					if (m_grid[GetGridIndex(ptAhead)] == Grid::TILE_USED)
					{
						return false;
					}
				}
			}
			for (int zAxis = 0; zAxis < (go->iGridZ - iHalfGridZ); ++zAxis)
			{
				ptAhead.Set(next.x - iHalfGridX, next.z - zAxis);
				if (isPointInGrid(ptAhead))
				{
					if (m_grid[GetGridIndex(ptAhead)] == Grid::TILE_USED)
					{
						return false;
					}
				}
			}
			break;
		case Grid::DIR_RIGHT:
			for (int zAxis = 1; zAxis <= iHalfGridZ; ++zAxis)
			{
				ptAhead.Set(next.x + iHalfGridX, next.z + zAxis);
				if (isPointInGrid(ptAhead))
				{
					if (m_grid[GetGridIndex(ptAhead)] == Grid::TILE_USED)
					{
						return false;
					}
				}
			}
			for (int zAxis = 0; zAxis < (go->iGridZ - iHalfGridZ); ++zAxis)
			{
				ptAhead.Set(next.x + iHalfGridX, next.z - zAxis);
				if (isPointInGrid(ptAhead))
				{
					if (m_grid[GetGridIndex(ptAhead)] == Grid::TILE_USED)
					{
						return false;
					}
				}
			}
			break;
		case Grid::DIR_UP:
			for (int xAxis = 1; xAxis <= iHalfGridX; ++xAxis)
			{
				ptAhead.Set(next.x + xAxis, next.z - iHalfGridZ);
				if (isPointInGrid(ptAhead))
				{
					if (m_grid[GetGridIndex(ptAhead)] == Grid::TILE_USED)
					{
						return false;
					}
				}
			}

			for (int xAxis = 0; xAxis < (go->iGridX - iHalfGridX); ++xAxis)
			{
				ptAhead.Set(next.x - xAxis, next.z - iHalfGridZ);
				if (isPointInGrid(ptAhead))
				{
					if (m_grid[GetGridIndex(ptAhead)] == Grid::TILE_USED)
					{
						return false;
					}
				}
			}
			break;
		case Grid::DIR_DOWN:
			for (int xAxis = 1; xAxis <= iHalfGridX; ++xAxis)
			{
				ptAhead.Set(next.x + xAxis, next.z + iHalfGridZ);
				if (isPointInGrid(ptAhead))
				{
					if (m_grid[GetGridIndex(ptAhead)] == Grid::TILE_USED)
					{
						return false;
					}
				}
			}
			for (int xAxis = 0; xAxis < (go->iGridX - iHalfGridX); ++xAxis)
			{
				ptAhead.Set(next.x - xAxis, next.z + iHalfGridZ);
				if (isPointInGrid(ptAhead))
				{
					if (m_grid[GetGridIndex(ptAhead)] == Grid::TILE_USED)
					{
						return false;
					}
				}
			}
			break;
		case Grid::DIR_LEFTUP:
			//Checking Left
			for (int zAxis = 1; zAxis <= iHalfGridZ; ++zAxis)
			{
				ptAhead.Set(next.x - iHalfGridX, next.z + zAxis);
				if (isPointInGrid(ptAhead))
				{
					if (m_grid[GetGridIndex(ptAhead)] == Grid::TILE_USED)
					{
						return false;
					}
				}
			}
			for (int zAxis = 0; zAxis < (go->iGridZ - iHalfGridZ); ++zAxis)
			{
				ptAhead.Set(next.x - iHalfGridX, next.z - zAxis);
				if (isPointInGrid(ptAhead))
				{
					if (m_grid[GetGridIndex(ptAhead)] == Grid::TILE_USED)
					{
						return false;
					}
				}
			}
			//Checking Up
			for (int xAxis = 1; xAxis <= iHalfGridX; ++xAxis)
			{
				ptAhead.Set(next.x + xAxis, next.z - iHalfGridZ);
				if (isPointInGrid(ptAhead))
				{
					if (m_grid[GetGridIndex(ptAhead)] == Grid::TILE_USED)
					{
						return false;
					}
				}
			}
			for (int xAxis = 0; xAxis < (go->iGridX - iHalfGridX); ++xAxis)
			{
				ptAhead.Set(next.x - xAxis, next.z - iHalfGridZ);
				if (isPointInGrid(ptAhead))
				{
					if (m_grid[GetGridIndex(ptAhead)] == Grid::TILE_USED)
					{
						return false;
					}
				}
			}
			break;
		case Grid::DIR_RIGHTUP:
			//Checking Right
			for (int zAxis = 1; zAxis <= iHalfGridZ; ++zAxis)
			{
				ptAhead.Set(next.x + iHalfGridX, next.z + zAxis);
				if (isPointInGrid(ptAhead))
				{
					if (m_grid[GetGridIndex(ptAhead)] == Grid::TILE_USED)
					{
						return false;
					}
				}
			}
			for (int zAxis = 0; zAxis < (go->iGridZ - iHalfGridZ); ++zAxis)
			{
				ptAhead.Set(next.x + iHalfGridX, next.z - zAxis);
				if (isPointInGrid(ptAhead))
				{
					if (m_grid[GetGridIndex(ptAhead)] == Grid::TILE_USED)
					{
						return false;
					}
				}
			}
			//Checking Up
			for (int xAxis = 1; xAxis <= iHalfGridX; ++xAxis)
			{
				ptAhead.Set(next.x + xAxis, next.z - iHalfGridZ);
				if (isPointInGrid(ptAhead))
				{
					if (m_grid[GetGridIndex(ptAhead)] == Grid::TILE_USED)
					{
						return false;
					}
				}
			}
			for (int xAxis = 0; xAxis < (go->iGridX - iHalfGridX); ++xAxis)
			{
				ptAhead.Set(next.x - xAxis, next.z - iHalfGridZ);
				if (isPointInGrid(ptAhead))
				{
					if (m_grid[GetGridIndex(ptAhead)] == Grid::TILE_USED)
					{
						return false;
					}
				}
			}
			break;
		case Grid::DIR_LEFTDOWN:
			//Checking Left
			for (int zAxis = 1; zAxis <= iHalfGridZ; ++zAxis)
			{
				ptAhead.Set(next.x - iHalfGridX, next.z + zAxis);
				if (isPointInGrid(ptAhead))
				{
					if (m_grid[GetGridIndex(ptAhead)] == Grid::TILE_USED)
					{
						return false;
					}
				}
			}
			for (int zAxis = 0; zAxis < (go->iGridZ - iHalfGridZ); ++zAxis)
			{
				ptAhead.Set(next.x - iHalfGridX, next.z - zAxis);
				if (isPointInGrid(ptAhead))
				{
					if (m_grid[GetGridIndex(ptAhead)] == Grid::TILE_USED)
					{
						return false;
					}
				}
			}
			//Checking Down
			for (int xAxis = 1; xAxis <= iHalfGridX; ++xAxis)
			{
				ptAhead.Set(next.x + xAxis, next.z + iHalfGridZ);
				if (isPointInGrid(ptAhead))
				{
					if (m_grid[GetGridIndex(ptAhead)] == Grid::TILE_USED)
					{
						return false;
					}
				}
			}
			for (int xAxis = 0; xAxis < (go->iGridX - iHalfGridX); ++xAxis)
			{
				ptAhead.Set(next.x - xAxis, next.z + iHalfGridZ);
				if (isPointInGrid(ptAhead))
				{
					if (m_grid[GetGridIndex(ptAhead)] == Grid::TILE_USED)
					{
						return false;
					}
				}
			}
			break;
		case Grid::DIR_RIGHTDOWN:
			//Checking Right
			for (int zAxis = 1; zAxis <= iHalfGridZ; ++zAxis)
			{
				ptAhead.Set(next.x + iHalfGridX, next.z + zAxis);
				if (isPointInGrid(ptAhead))
				{
					if (m_grid[GetGridIndex(ptAhead)] == Grid::TILE_USED)
					{
						return false;
					}
				}
			}
			for (int zAxis = 0; zAxis < (go->iGridZ - iHalfGridZ); ++zAxis)
			{
				ptAhead.Set(next.x + iHalfGridX, next.z - zAxis);
				if (isPointInGrid(ptAhead))
				{
					if (m_grid[GetGridIndex(ptAhead)] == Grid::TILE_USED)
					{
						return false;
					}
				}
			}
			//Checking Down
			for (int xAxis = 1; xAxis <= iHalfGridX; ++xAxis)
			{
				ptAhead.Set(next.x + xAxis, next.z + iHalfGridZ);
				if (isPointInGrid(ptAhead))
				{
					if (m_grid[GetGridIndex(ptAhead)] == Grid::TILE_USED)
					{
						return false;
					}
				}
			}
			for (int xAxis = 0; xAxis < (go->iGridX - iHalfGridX); ++xAxis)
			{
				ptAhead.Set(next.x - xAxis, next.z + iHalfGridZ);
				if (isPointInGrid(ptAhead))
				{
					if (m_grid[GetGridIndex(ptAhead)] == Grid::TILE_USED)
					{
						return false;
					}
				}
			}
			break;
		default:
			break;
		}
	}
	return true;
}

void SceneSP::ChangeState(GAME_STATE newstate)
{
	switch (newstate)
	{
	case G_SPLASHSCREEN:
		camera.Init(Vector3(0, 0, 1), Vector3(0, 0, 0), Vector3(0, 1, 0));	// splashscreen
		break;
	case G_MAINMENU:
		camera.Init(Vector3(0, 0, 1), Vector3(0, 0, 0), Vector3(0, 1, 0));	// splashscreen
		UIManager::GetInstance()->AddUI("startButton", new UIMenuButton("start"));
		break;
	case G_INPLAY:
		camera.Init(Vector3(0, 2, 2), Vector3(0, 0, 0), Vector3(0, 1, 0));	// game
		UIManager::GetInstance()->GetUI("startButton")->bIsDone = true;
		UIManager::GetInstance()->AddUI("uiReligionBar", new UIReligionBar());
		break;
	default:
		break;
	}

	Vector3 dir = camera.target - camera.position;
	dir.Normalize();
	Vector3 right(1, 0, 0);
	camera.up = right.Cross(dir);
	camera.fDistance = (camera.target - camera.position).Length();

	game_state = newstate;
}

void SceneSP::Init()
{
	SceneData::GetInstance()->SetNoGrid(15);
	SceneData::GetInstance()->SetGridSize(1.f);
	SceneData::GetInstance()->SetGridOffset(0.5f);
	SceneBase::Init();

	m_grid.resize(SceneData::GetInstance()->GetNoGrid() * SceneData::GetInstance()->GetNoGrid());
	std::fill(m_grid.begin(), m_grid.end(), Grid::TILE_EMPTY);
	bShowGrid = false;
	bGodlights = true;
	//Calculating aspect ratio
	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();
	SceneData::GetInstance()->SetWorldHeight(m_worldHeight);
	SceneData::GetInstance()->SetWorldWidth(m_worldWidth);
	SceneData::GetInstance()->SetElapsedTime(0);
	SceneData::GetInstance()->SetReligionValue(0.f);
	SceneData::GetInstance()->SetMaxReligionValue(100.f);
	PostOffice::GetInstance()->Register("Scene", this);

	//Physics code here
	m_speed = 1.f;

	Math::InitRNG();

	selected = NULL;

	//Objects from maya, bottom of object to be translated down
	goVillager = FetchGO(GameObject::GO_VILLAGER);
	goVillager->scale.y = 1.f;
	goVillager->pos.Set(0, goVillager->scale.y * 0.5f, 0);
	goVillager->iGridX = 1;
	goVillager->iGridZ = 1;
	goVillager->pos = GetGridPos(GridPt(5, 5));
	goVillager->pos.y = goVillager->scale.y * 0.5f;

	goChiefHut = FetchGO(GameObject::GO_CHIEFHUT);
	goChiefHut->pos = GetGridPos(GridPt(8, 7));
	goChiefHut->pos.y = goChiefHut->scale.y * 0.5f;
	static_cast<Building*>(goChiefHut)->bBuilt = true;

	goAltar = FetchGO(GameObject::GO_ALTAR);
	goAltar->pos = GetGridPos(GridPt(6, 2));
	goAltar->pos.y = goChiefHut->scale.y * 0.5f;
	Altar* altar = static_cast<Altar*>(goAltar);
	altar->bBuilt = true;
	altar->iFoodOffered = 0;

	goBush = FetchGO(GameObject::GO_BUSH);
	goBush->pos = GetGridPos(GridPt(1, 1));
	goBush->pos.y = goBush->scale.y * 0.5f;
	Bush* bGo = static_cast<Bush*>(goBush);
	bGo->eCurrState = Bush::LUSH;
	bGo->fTimer = 0;
	bGo->iFoodAmount = 10;

	goTree = FetchGO(GameObject::GO_TREE);
	goTree->pos = GetGridPos(GridPt(2, 8));
	goTree->pos.y = goTree->scale.y * 0.5f;
	goTree->iGridX = 1;
	goTree->iGridZ = 1;
	Tree* tGo = static_cast<Tree*>(goTree);
	tGo->eCurrState = Tree::FULL;
	tGo->fTimer = 0;
	tGo->iWoodAmount = 10;

	goTree2 = FetchGO(GameObject::GO_TREE);
	goTree2->pos = GetGridPos(GridPt(2, 9));
	goTree2->pos.y = goTree2->scale.y * 0.5f;
	goTree2->iGridX = 1;
	goTree2->iGridZ = 1;
	tGo = static_cast<Tree*>(goTree2);
	tGo->eCurrState = Tree::FULL;
	tGo->fTimer = 0;
	tGo->iWoodAmount = 10;

	SceneData* SD = SceneData::GetInstance();
	SD->SetFood(0);
	SD->SetFoodLimit(100);
	SD->SetPopulation(0);
	SD->SetPopulationLimit(10);
	SD->SetWood(0);
	SD->SetWoodLimit(100);
	SD->SetCurrMonth(1);
	SD->SetCurrDay(1);

	bDay = true; // day
	fTimeOfDay = 8.f;
	bGoalAchieved = false;

	//go->vel.Set(1, 0, 0);
	MousePicker::GetInstance()->Init();
	MousePicker::GetInstance()->SetProjectionStack(projectionStack);
	MousePicker::GetInstance()->SetViewStack(viewStack);

	UIManager::GetInstance()->Init();

	EffectManager::GetInstance()->Init();
	EffectManager::GetInstance()->AddEffect(new EffectTrail(&camera));
	EffectManager::GetInstance()->AddEffect(new EffectHand());

	CalamityManager::GetInstance()->Init();

	ChangeState(G_SPLASHSCREEN);
}


bool SceneSP::Handle(Message* message)
{
	MessageWRU* messageWRU = dynamic_cast<MessageWRU*>(message);
	if (messageWRU)
	{
		switch (messageWRU->type)
		{
		case MessageWRU::FIND_CHIEFHUT:
			messageWRU->go->goTarget = goChiefHut;
			break;
		case MessageWRU::PATH_TO_TARGET:
			AStarSingleGrid(messageWRU->go, GetPoint(messageWRU->go->goTarget->pos));
			break;
		case MessageWRU::PATH_TO_POINT:
			AStarSingleGrid(messageWRU->go, GetPoint(messageWRU->go->target));
			break;
		case MessageWRU::RANDOM_TARGET:
		{
			float fLengthAway = 0;
			int iRandIndex;
			GridPt currGrid = GetPoint(messageWRU->go->pos);
			while (fLengthAway < messageWRU->threshold)
			{
				iRandIndex = Math::RandIntMinMax(0, SceneData::GetInstance()->GetNoGrid() * SceneData::GetInstance()->GetNoGrid() - 1);
				if (m_grid[iRandIndex] == Grid::TILE_EMPTY)
				{
					std::pair<int, int> randPoint = GetPoint(iRandIndex);
					fLengthAway = (randPoint.first - currGrid.x) * (randPoint.first - currGrid.x) + (randPoint.second - currGrid.z) * (randPoint.second - currGrid.z);
				}
			}
			std::pair<int, int> randPoint = GetPoint(iRandIndex);
			messageWRU->go->target = GetGridPos(GridPt(randPoint.first,randPoint.second));
		}
			break;
		default:
			break;
		}
		delete message;
		return true;
	}
	MessageCameraShake* messageCamShake = dynamic_cast<MessageCameraShake*>(message);
	if (messageCamShake)
	{
		camera.SetCamShake(messageCamShake->type + 1, messageCamShake->intensity, messageCamShake->duration);
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
				go->scale.Set(SceneData::GetInstance()->GetGridSize() * 0.5f, 0.25f, SceneData::GetInstance()->GetGridSize() * 0.5f);
				break;
			case GameObject::GO_BUSH:
				go->scale.Set(SceneData::GetInstance()->GetGridSize() * 0.75f, 1.f, SceneData::GetInstance()->GetGridSize() * 0.75f);
				break;
			case GameObject::GO_CHIEFHUT:
				go->scale.Set(SceneData::GetInstance()->GetGridSize() * 1.f, 0.5f, SceneData::GetInstance()->GetGridSize() * 1.f);
				break;
			case GameObject::GO_TREE:
				go->scale.Set(SceneData::GetInstance()->GetGridSize() * 1.f, 1.2f, SceneData::GetInstance()->GetGridSize() * 1.f);
				break;
			case GameObject::GO_ALTAR:
				go->scale.Set(SceneData::GetInstance()->GetGridSize() * 1.f, 2.5f, SceneData::GetInstance()->GetGridSize() * 1.f);
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
		case GameObject::GO_ALTAR:
			go = new Altar(type);
			break;
		case GameObject::GO_BUSH:
			go = new Bush(type);
			break;
		case GameObject::GO_TREE:
			go = new Tree(type);
			break;
		default:
			go = new GameObject(type);
			break;
		}
		if (go != nullptr)
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

struct Compare3
{
	bool operator()(std::pair<GridPt, std::pair<float, float>> pair1, std::pair<GridPt, std::pair<float, float>> pair2)
	{
		return pair1.second.second < pair2.second.second;
	}
};

void SceneSP::AStarSingleGrid(GameObject * go, GridPt target)
{

	//first = curr point, second.first = tile cost, second.second = total cost(tile cost + distance to final destination)
	if (go->currentPt == target)
		return;
	std::vector<std::pair<GridPt, std::pair<float, float>>>priority_Queue;
	std::sort(priority_Queue.begin(), priority_Queue.end(), Compare3());
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

	GridPt curr = go->currentPt;
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
		GridPt temp;
		//Check right of gridPt
		if (go->pos.x > ptPos.x)
		{
			temp = GridPt(curr.x + 1, curr.z);
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
			if (go->pos.x > ptPos.x)
			{
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
		}
		else if (go->pos.z > ptPos.z)
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

	float G = 0;
	priority_Queue.push_back(std::pair<GridPt, std::pair<float, float>>(bestStart, std::pair<float, float>(G, 0)));
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
		if ((target.x - curr.x) * (target.x - curr.x) <= 1 && (target.z - curr.z) * (target.z - curr.z) <= 1)
		{
			if (target != curr)
				m_previous[GetGridIndex(target)] = curr;
			priority_Queue.begin()->first = target;
			break;
		}

		//Check Up
		G = priority_Queue.begin()->second.first;
		next.Set(curr.x, curr.z - 1);
		currIndex = GetGridIndex(next);
		if (isPointInGrid(next))
		{
			float distanceSquared = NULL;
			//Cost of Empty Tile
			G += 1;
			//Calculate distance to end
			distanceSquared = (int)Math::FAbs(target.x - next.x) + (int)Math::FAbs(target.z - next.z);
			if (m_grid[currIndex] == Grid::TILE_EMPTY)
			{
				if (!go->m_visited[currIndex])
				{
					//Update node cost
					m_previous[currIndex] = curr;
					priority_Queue.push_back(std::pair<GridPt, std::pair<float, float>>(next, std::pair<float, float>(G, G + distanceSquared)));
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
					for (std::vector<std::pair<GridPt, std::pair<float, float>>>::iterator it = priority_Queue.begin(); it != priority_Queue.end(); ++it)
					{
						std::pair<GridPt, std::pair<float, float>> path = *it;
						if (path.first == next)
						{
							//If in queue, check if new path is shorter thn old path
							if (path.second.second > G + distanceSquared)
							{
								//Update if new path is shorter
								priority_Queue.erase(it);
								priority_Queue.push_back(std::pair<GridPt, std::pair<float, float>>(next, std::pair<float, float>(G, G + distanceSquared)));
								m_previous[currIndex] = curr;
							}
							break;
						}
					}
					//If not in queue, node is already in optimal path
				}
			}
		}

		//Check Down
		G = priority_Queue.begin()->second.first;
		next.Set(curr.x, curr.z + 1);
		currIndex = GetGridIndex(next);
		if (isPointInGrid(next))
		{
			float distanceSquared = NULL;
			//Cost of Empty Tile
			G += 1;
			//Calculate distance to end
			distanceSquared = (int)Math::FAbs(target.x - next.x) + (int)Math::FAbs(target.z - next.z);
			if (m_grid[currIndex] == Grid::TILE_EMPTY)
			{
				if (!go->m_visited[currIndex])
				{
					//Update node cost
					m_previous[currIndex] = curr;
					priority_Queue.push_back(std::pair<GridPt, std::pair<float, float>>(next, std::pair<float, float>(G, G + distanceSquared)));
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
					for (std::vector<std::pair<GridPt, std::pair<float, float>>>::iterator it = priority_Queue.begin(); it != priority_Queue.end(); ++it)
					{
						std::pair<GridPt, std::pair<float, float>> path = *it;
						if (path.first == next)
						{
							//If in queue, check if new path is shorter thn old path
							if (path.second.second > G + distanceSquared)
							{
								//Update if new path is shorter
								priority_Queue.erase(it);
								priority_Queue.push_back(std::pair<GridPt, std::pair<float, float>>(next, std::pair<float, float>(G, G + distanceSquared)));
								m_previous[currIndex] = curr;
							}
							break;
						}
					}
					//If not in queue, node is already in optimal path
				}
			}
		}

		//Check Left
		G = priority_Queue.begin()->second.first;
		next.Set(curr.x - 1, curr.z);
		currIndex = GetGridIndex(next);
		if (isPointInGrid(next))
		{
			float distanceSquared = NULL;
			//Cost of Empty Tile
			G += 1;
			//Calculate distance to end
			distanceSquared = (int)Math::FAbs(target.x - next.x) + (int)Math::FAbs(target.z - next.z);
			if (m_grid[currIndex] == Grid::TILE_EMPTY)
			{
				if (!go->m_visited[currIndex])
				{
					//Update node cost
					m_previous[currIndex] = curr;
					priority_Queue.push_back(std::pair<GridPt, std::pair<float, float>>(next, std::pair<float, float>(G, G + distanceSquared)));
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
					for (std::vector<std::pair<GridPt, std::pair<float, float>>>::iterator it = priority_Queue.begin(); it != priority_Queue.end(); ++it)
					{
						std::pair<GridPt, std::pair<float, float>> path = *it;
						if (path.first == next)
						{
							//If in queue, check if new path is shorter thn old path
							if (path.second.second > G + distanceSquared)
							{
								//Update if new path is shorter
								priority_Queue.erase(it);
								priority_Queue.push_back(std::pair<GridPt, std::pair<float, float>>(next, std::pair<float, float>(G, G + distanceSquared)));
								m_previous[currIndex] = curr;
							}
							break;
						}
					}
					//If not in queue, node is already in optimal path
				}
			}
		}

		//Check Right
		G = priority_Queue.begin()->second.first;
		next.Set(curr.x + 1, curr.z);
		currIndex = GetGridIndex(next);
		if (isPointInGrid(next))
		{
			float distanceSquared = NULL;
			//Cost of Empty Tile
			G += 1;
			//Calculate distance to end
			distanceSquared = (int)Math::FAbs(target.x - next.x) + (int)Math::FAbs(target.z - next.z);
			if (m_grid[currIndex] == Grid::TILE_EMPTY)
			{
				if (!go->m_visited[currIndex])
				{
					//Update node cost
					m_previous[currIndex] = curr;
					priority_Queue.push_back(std::pair<GridPt, std::pair<float, float>>(next, std::pair<float, float>(G, G + distanceSquared)));
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
					for (std::vector<std::pair<GridPt, std::pair<float, float>>>::iterator it = priority_Queue.begin(); it != priority_Queue.end(); ++it)
					{
						std::pair<GridPt, std::pair<float, float>> path = *it;
						if (path.first == next)
						{
							//If in queue, check if new path is shorter thn old path
							if (path.second.second > G + distanceSquared)
							{
								//Update if new path is shorter
								priority_Queue.erase(it);
								priority_Queue.push_back(std::pair<GridPt, std::pair<float, float>>(next, std::pair<float, float>(G, G + distanceSquared)));
								m_previous[currIndex] = curr;
							}
							break;
						}
					}
					//If not in queue, node is already in optimal path
				}
			}
		}

		//Check Up Left
		G = priority_Queue.begin()->second.first;
		next.Set(curr.x - 1, curr.z - 1);
		currIndex = GetGridIndex(next);
		if (isPointInGrid(next))
		{
			float distanceSquared = NULL;
			//Cost of Empty Tile
			G += sqrtf(2);
			//Calculate distance to end
			distanceSquared = (int)Math::FAbs(target.x - next.x) + (int)Math::FAbs(target.z - next.z);
			if (m_grid[currIndex] == Grid::TILE_EMPTY)
			{
				if (!go->m_visited[currIndex])
				{
					//Update node cost
					m_previous[currIndex] = curr;
					priority_Queue.push_back(std::pair<GridPt, std::pair<float, float>>(next, std::pair<float, float>(G, G + distanceSquared)));
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
					for (std::vector<std::pair<GridPt, std::pair<float, float>>>::iterator it = priority_Queue.begin(); it != priority_Queue.end(); ++it)
					{
						std::pair<GridPt, std::pair<float, float>> path = *it;
						if (path.first == next)
						{
							//If in queue, check if new path is shorter thn old path
							if (path.second.second > G + distanceSquared)
							{
								//Update if new path is shorter
								priority_Queue.erase(it);
								priority_Queue.push_back(std::pair<GridPt, std::pair<float, float>>(next, std::pair<float, float>(G, G + distanceSquared)));
								m_previous[currIndex] = curr;
							}
							break;
						}
					}
					//If not in queue, node is already in optimal path
				}
			}
		}

		//Check Up Right
		G = priority_Queue.begin()->second.first;
		next.Set(curr.x + 1, curr.z - 1);
		currIndex = GetGridIndex(next);
		if (isPointInGrid(next))
		{
			float distanceSquared = NULL;
			//Cost of Empty Tile
			G += sqrtf(2);
			//Calculate distance to end
			distanceSquared = (int)Math::FAbs(target.x - next.x) + (int)Math::FAbs(target.z - next.z);
			if (m_grid[currIndex] == Grid::TILE_EMPTY)
			{
				if (!go->m_visited[currIndex])
				{
					//Update node cost
					m_previous[currIndex] = curr;
					priority_Queue.push_back(std::pair<GridPt, std::pair<float, float>>(next, std::pair<float, float>(G, G + distanceSquared)));
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
					for (std::vector<std::pair<GridPt, std::pair<float, float>>>::iterator it = priority_Queue.begin(); it != priority_Queue.end(); ++it)
					{
						std::pair<GridPt, std::pair<float, float>> path = *it;
						if (path.first == next)
						{
							//If in queue, check if new path is shorter thn old path
							if (path.second.second > G + distanceSquared)
							{
								//Update if new path is shorter
								priority_Queue.erase(it);
								priority_Queue.push_back(std::pair<GridPt, std::pair<float, float>>(next, std::pair<float, float>(G, G + distanceSquared)));
								m_previous[currIndex] = curr;
							}
							break;
						}
					}
					//If not in queue, node is already in optimal path
				}
			}
		}

		//Check Down Left
		G = priority_Queue.begin()->second.first;
		next.Set(curr.x - 1, curr.z + 1);
		currIndex = GetGridIndex(next);
		if (isPointInGrid(next))
		{
			float distanceSquared = NULL;
			//Cost of Empty Tile
			G += sqrtf(2);
			//Calculate distance to end
			distanceSquared = (int)Math::FAbs(target.x - next.x) + (int)Math::FAbs(target.z - next.z);
			if (m_grid[currIndex] == Grid::TILE_EMPTY)
			{
				if (!go->m_visited[currIndex])
				{
					//Update node cost
					m_previous[currIndex] = curr;
					priority_Queue.push_back(std::pair<GridPt, std::pair<float, float>>(next, std::pair<float, float>(G, G + distanceSquared)));
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
					for (std::vector<std::pair<GridPt, std::pair<float, float>>>::iterator it = priority_Queue.begin(); it != priority_Queue.end(); ++it)
					{
						std::pair<GridPt, std::pair<float, float>> path = *it;
						if (path.first == next)
						{
							//If in queue, check if new path is shorter thn old path
							if (path.second.second > G + distanceSquared)
							{
								//Update if new path is shorter
								priority_Queue.erase(it);
								priority_Queue.push_back(std::pair<GridPt, std::pair<float, float>>(next, std::pair<float, float>(G, G + distanceSquared)));
								m_previous[currIndex] = curr;
							}
							break;
						}
					}
					//If not in queue, node is already in optimal path
				}
			}
		}

		//Check Down Right
		G = priority_Queue.begin()->second.first;
		next.Set(curr.x + 1, curr.z + 1);
		currIndex = GetGridIndex(next);
		if (isPointInGrid(next))
		{
			float distanceSquared = NULL;
			//Cost of Empty Tile
			G += sqrtf(2);
			//Calculate distance to end
			distanceSquared = (int)Math::FAbs(target.x - next.x) + (int)Math::FAbs(target.z - next.z);
			if (m_grid[currIndex] == Grid::TILE_EMPTY)
			{
				if (!go->m_visited[currIndex])
				{
					//Update node cost
					m_previous[currIndex] = curr;
					priority_Queue.push_back(std::pair<GridPt, std::pair<float, float>>(next, std::pair<float, float>(G, G + distanceSquared)));
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
					for (std::vector<std::pair<GridPt, std::pair<float, float>>>::iterator it = priority_Queue.begin(); it != priority_Queue.end(); ++it)
					{
						std::pair<GridPt, std::pair<float, float>> path = *it;
						if (path.first == next)
						{
							//If in queue, check if new path is shorter thn old path
							if (path.second.second > G + distanceSquared)
							{
								//Update if new path is shorter
								priority_Queue.erase(it);
								priority_Queue.push_back(std::pair<GridPt, std::pair<float, float>>(next, std::pair<float, float>(G, G + distanceSquared)));
								m_previous[currIndex] = curr;
							}
							break;
						}
					}
					//If not in queue, node is already in optimal path
				}
			}
		}

		priority_Queue.erase(priority_Queue.begin());
		std::sort(priority_Queue.begin(), priority_Queue.end(), Compare3());
	}

	if (priority_Queue.size() > 0)
	{
		if (priority_Queue.begin()->first == target)
		{
			//If manage to reach target
			curr = target;
			m_shortestPath.push_back(curr);
			curr = m_previous[GetGridIndex(curr)];
			while (curr != NULL)
			{
				m_shortestPath.push_back(curr);
				curr = m_previous[GetGridIndex(curr)];
				std::cout << "hi" << std::endl;
			}
		}
		else
		{
			//If unable to reach target
			curr = nearestTile;
			m_shortestPath.push_back(curr);
			curr = m_previous[GetGridIndex(curr)];
			while (curr != NULL)
			{
				m_shortestPath.push_back(curr);
				curr = m_previous[GetGridIndex(curr)];
				std::cout << "hi" << std::endl;
			}
		}
	}
	else
	{
		//If something happened
		curr = nearestTile;
		m_shortestPath.push_back(curr);
		curr = m_previous[GetGridIndex(curr)];
		while (curr != NULL)
		{
			m_shortestPath.push_back(curr);
			curr = m_previous[GetGridIndex(curr)];
			std::cout << "hi" << std::endl;
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

void SceneSP::AStarMultiGrid(GameObject * go, GridPt target)
{

	//first = curr point, second.first = tile cost, second.second = total cost(tile cost + distance to final destination)
	if (go->currentPt == target)
		return;
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

	GridPt curr = go->currentPt;
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
			if (isTheCoastClear(go, temp, Grid::DIR_LEFT))
			{
				float distance = (target.x - temp.x) * (target.x - temp.x) + (target.z - temp.z) * (target.z - temp.z);
				if (distance < fDistanceToTarget)
				{
					fDistanceToTarget = distance;
					bestStart = temp;
				}
			}
		}
		//Check if goPos is above or below center of gridPt
		if (go->pos.z < ptPos.z)
		{
			//Check above of gridPt
			temp = GridPt(curr.x, curr.z - 1);
			if (isPointInGrid(temp))
			{
				if (isTheCoastClear(go, temp, Grid::DIR_UP))
				{
					float distance = (target.x - temp.x) * (target.x - temp.x) + (target.z - temp.z) * (target.z - temp.z);
					if (distance < fDistanceToTarget)
					{
						fDistanceToTarget = distance;
						bestStart = temp;
					}
				}
			}
			//Check diagonal
			temp = GridPt(curr.x - 1, curr.z - 1);
			if (isPointInGrid(temp))
			{
				if (isTheCoastClear(go, temp, Grid::DIR_LEFTUP))
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
			//Check below of gridPt
			temp = GridPt(curr.x, curr.z + 1);
			if (isPointInGrid(temp))
			{
				if (isTheCoastClear(go, temp, Grid::DIR_DOWN))
				{
					float distance = (target.x - temp.x) * (target.x - temp.x) + (target.z - temp.z) * (target.z - temp.z);
					if (distance < fDistanceToTarget)
					{
						fDistanceToTarget = distance;
						bestStart = temp;
					}
				}
			}
			//Check diagonal
			temp = GridPt(curr.x - 1, curr.z + 1);
			if (isPointInGrid(temp))
			{
				if (isTheCoastClear(go, temp, Grid::DIR_LEFTDOWN))
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
	}
	else
	{
		GridPt temp;
		//Check right of gridPt
		if (go->pos.x > ptPos.x)
		{
			temp = GridPt(curr.x + 1, curr.z);
			if (isPointInGrid(temp))
			{
				if (isTheCoastClear(go, temp, Grid::DIR_RIGHT))
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
		//Check if goPos is above or below center of gridPt
		if (go->pos.z < ptPos.z)
		{
			//Check above of gridPt
			temp = GridPt(curr.x, curr.z - 1);
			if (isPointInGrid(temp))
			{
				if (isTheCoastClear(go, temp, Grid::DIR_UP))
				{
					float distance = (target.x - temp.x) * (target.x - temp.x) + (target.z - temp.z) * (target.z - temp.z);
					if (distance < fDistanceToTarget)
					{
						fDistanceToTarget = distance;
						bestStart = temp;
					}
				}
			}
			//Check diagonal
			if (go->pos.x > ptPos.x)
			{
				temp = GridPt(curr.x + 1, curr.z - 1);
				if (isPointInGrid(temp))
				{
					if (isTheCoastClear(go, temp, Grid::DIR_RIGHTUP))
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
		}
		else if (go->pos.z > ptPos.z)
		{
			//Check below of gridPt
			temp = GridPt(curr.x, curr.z + 1);
			if (isPointInGrid(temp))
			{
				if (isTheCoastClear(go, temp, Grid::DIR_DOWN))
				{
					float distance = (target.x - temp.x) * (target.x - temp.x) + (target.z - temp.z) * (target.z - temp.z);
					if (distance < fDistanceToTarget)
					{
						fDistanceToTarget = distance;
						bestStart = temp;
					}
				}
			}
			//Check diagonal
			temp = GridPt(curr.x + 1, curr.z + 1);
			if (isPointInGrid(temp))
			{
				if (isTheCoastClear(go, temp, Grid::DIR_RIGHTDOWN))
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
	}

	int G = 0;
	priority_Queue.push_back(std::pair<GridPt, std::pair<int, int>>(bestStart, std::pair<int, int>(G, 0)));
	go->m_visited[GetGridIndex(bestStart)] = true;
	float nearestDistance = FLT_MAX;
	GridPt nearestTile = bestStart;
	GridPt next = bestStart;
	int currIndex = 0;

	//Since this value is rounded down, this value is for the closer side of GridPt, refer to gridImage in beside solution file
	int iHalfGridX = go->iGridX * 0.5f;
	int iHalfGridZ = go->iGridZ * 0.5f;


	for (int loop = 0; loop < SD->GetNoGrid() * SD->GetNoGrid() && !priority_Queue.empty(); ++loop)
	{
		//std::cout << "One Round of Loop" << std::endl;
 		curr = priority_Queue.begin()->first;
		//m_queue.pop();


		if (go->iGridX > 0 && go->iGridZ > 0)
		{
			if ((target.x - curr.x) * (target.x - curr.x) <= (go->iGridX - iHalfGridX + 1) * (go->iGridX - iHalfGridX + 1) && (target.z - curr.z) * (target.z - curr.z) <= (go->iGridZ - iHalfGridZ + 1) * (go->iGridZ - iHalfGridZ + 1))
			{
				if(target != curr)
					m_previous[GetGridIndex(target)] = curr;
				priority_Queue.begin()->first = target;
				break;
			}
		}
		else
		{
			if (curr == target)
			{
				break;
			}
		}

		//Check Up
		G = priority_Queue.begin()->second.first;
		next.Set(curr.x, curr.z - 1);
		currIndex = GetGridIndex(next);
		if (isPointInGrid(next))
		{
			float distanceSquared = NULL;
			//Cost of Empty Tile
			G += 1;
			//Calculate distance to end
			distanceSquared = (int)Math::FAbs(target.x - next.x) + (int)Math::FAbs(target.z - next.z);
			if (isTheCoastClear(go, next, Grid::DIR_UP))
			{
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
		}

		//Check Down
		G = priority_Queue.begin()->second.first;
		next.Set(curr.x, curr.z + 1);
		currIndex = GetGridIndex(next);
		if (isPointInGrid(next))
		{
			float distanceSquared = NULL;
			//Cost of Empty Tile
			G += 1;
			//Calculate distance to end
			distanceSquared = (int)Math::FAbs(target.x - next.x) + (int)Math::FAbs(target.z - next.z);
			if (isTheCoastClear(go, next, Grid::DIR_DOWN))
			{
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
		}

		//Check Left
		G = priority_Queue.begin()->second.first;
		next.Set(curr.x - 1, curr.z);
		currIndex = GetGridIndex(next);
		if (isPointInGrid(next))
		{
			float distanceSquared = NULL;
			//Cost of Empty Tile
			G += 1;
			//Calculate distance to end
			distanceSquared = (int)Math::FAbs(target.x - next.x) + (int)Math::FAbs(target.z - next.z);
			if (isTheCoastClear(go, next, Grid::DIR_LEFT))
			{
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
		}

		//Check Right
		G = priority_Queue.begin()->second.first;
		next.Set(curr.x + 1, curr.z);
		currIndex = GetGridIndex(next);
		if (isPointInGrid(next))
		{
			float distanceSquared = NULL;
			//Cost of Empty Tile
			G += 1;
			//Calculate distance to end
			distanceSquared = (int)Math::FAbs(target.x - next.x) + (int)Math::FAbs(target.z - next.z);
			if (isTheCoastClear(go, next, Grid::DIR_RIGHT))
			{
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
		}

		//Check Up Left
		G = priority_Queue.begin()->second.first;
		next.Set(curr.x - 1, curr.z - 1);
		currIndex = GetGridIndex(next);
		if (isPointInGrid(next))
		{
			float distanceSquared = NULL;
			//Cost of Empty Tile
			G += 1;
			//Calculate distance to end
			distanceSquared = (int)Math::FAbs(target.x - next.x) + (int)Math::FAbs(target.z - next.z);
			if (isTheCoastClear(go, next, Grid::DIR_LEFTUP))
			{
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
		}

		//Check Up Right
		G = priority_Queue.begin()->second.first;
		next.Set(curr.x + 1, curr.z - 1);
		currIndex = GetGridIndex(next);
		if (isPointInGrid(next))
		{
			float distanceSquared = NULL;
			//Cost of Empty Tile
			G += 1;
			//Calculate distance to end
			distanceSquared = (int)Math::FAbs(target.x - next.x) + (int)Math::FAbs(target.z - next.z);
			if (isTheCoastClear(go, next, Grid::DIR_RIGHTUP))
			{
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
		}

		//Check Down Left
		G = priority_Queue.begin()->second.first;
		next.Set(curr.x - 1, curr.z + 1);
		currIndex = GetGridIndex(next);
		if (isPointInGrid(next))
		{
			float distanceSquared = NULL;
			//Cost of Empty Tile
			G += 1;
			//Calculate distance to end
			distanceSquared = (int)Math::FAbs(target.x - next.x) + (int)Math::FAbs(target.z - next.z);
			if (isTheCoastClear(go, next, Grid::DIR_LEFTDOWN))
			{
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
		}

		//Check Down Right
		G = priority_Queue.begin()->second.first;
		next.Set(curr.x + 1, curr.z + 1);
		currIndex = GetGridIndex(next);
		if (isPointInGrid(next))
		{
			float distanceSquared = NULL;
			//Cost of Empty Tile
			G += 1;
			//Calculate distance to end
			distanceSquared = (int)Math::FAbs(target.x - next.x) + (int)Math::FAbs(target.z - next.z);
			if (isTheCoastClear(go, next, Grid::DIR_RIGHTDOWN))
			{
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
		}

		priority_Queue.erase(priority_Queue.begin());
		std::sort(priority_Queue.begin(), priority_Queue.end(), Compare2());
	}

	if (priority_Queue.size() > 0)
	{
		if (priority_Queue.begin()->first == target)
		{
			//If manage to reach target
			curr = target;
			m_shortestPath.push_back(curr);
			curr = m_previous[GetGridIndex(curr)];
			while (curr != NULL)
			{
				m_shortestPath.push_back(curr);
				curr = m_previous[GetGridIndex(curr)];
				std::cout << "hi" << std::endl;
			}
		}
		else
		{
			//If unable to reach target
			curr = nearestTile;
			m_shortestPath.push_back(curr);
			curr = m_previous[GetGridIndex(curr)];
			while (curr != NULL)
			{
				m_shortestPath.push_back(curr);
				curr = m_previous[GetGridIndex(curr)];
				std::cout << "hi" << std::endl;
			}
		}
	}
	else
	{
		//If something happened
		curr = nearestTile;
		m_shortestPath.push_back(curr);
		curr = m_previous[GetGridIndex(curr)];
		while (curr != NULL)
		{
			m_shortestPath.push_back(curr);
			curr = m_previous[GetGridIndex(curr)];
			std::cout << "hi" << std::endl;
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

	ChangeState(G_MAINMENU);
}

void SceneSP::ChangeTimeOfDay()
{
	bDay = !bDay;
	if (bDay) // daytime reset
	{
		//bGodlights = false;
		SceneData* SD = SceneData::GetInstance();
		SD->SetCurrDay(SD->GetCurrDay() + 1);
		for (auto go : m_goList)
		{
			if (!go->active)
				continue;
			if (go->type == GameObject::GO_BUSH)
			{
				if (static_cast<Bush*>(go)->eCurrState == Bush::DEPLETED)
					static_cast<Bush*>(go)->eCurrState = Bush::LUSH;
			}
		}
	}
	else // nighttime reset
	{
		//bGodlights = true;
		SceneData* SD = SceneData::GetInstance();
		SD->SetFood(Math::Max(0, SD->GetFood() - SD->GetPopulation() * 5));// 5 food is eaten per Villager
	}
}

void SceneSP::ProgressMonth()
{
	SceneData* SD = SceneData::GetInstance();
	SD->SetCurrMonth(SD->GetCurrMonth() + 1);
	if (SD->GetCurrMonth() >= 31)
	{
		SD->SetCurrMonth(1);
		//ChangeState(G_MAINMENU);
	}
	if (bGoalAchieved)
		bGoalAchieved = false;
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
	KeyboardController* KC = KeyboardController::GetInstance();
	UIManager* UIM = UIManager::GetInstance();
	EffectManager* EM = EffectManager::GetInstance();
	CalamityManager* CM = CalamityManager::GetInstance();

	//Calculating aspect ratio
	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();

	SD->SetWorldHeight(m_worldHeight);
	SD->SetWorldWidth(m_worldWidth);
	SD->SetElapsedTime(SD->GetElapsedTime() + (float)dt);

	SceneBase::Update(dt);
	MP->Update(dt);

	mousePos = MP->GetIntersectionWithPlane(camera.position, Vector3(0, 0, 0), Vector3(0, 1, 0));
	SD->SetMousePos_World(mousePos);

	CM->Update(dt);

	UIM->Update(dt);
	EM->Update(dt);

	switch (game_state)
	{
	case G_SPLASHSCREEN:
	{
		// made with opengl
		if (fOpenGLInTimer < 1.f) { fOpenGLInTimer += dt; }
		else
		{
			fOpenGLInTimer = 1.f;
			if (fOpenGLOutTimer > 0.f) { fOpenGLOutTimer -= dt * 0.75f; }
			else
			{
				fOpenGLOutTimer = 0.f;
				// game icon
				if (fSplashScreenInTimer < 1.f) { fSplashScreenInTimer += dt; }
				else
				{
					fSplashScreenInTimer = 1.f;
					/*if (fSplashScreenOutTimer > 0.f) { fSplashScreenOutTimer -= dt * 0.75f; }
					else*/
					{
						fSplashScreenOutTimer = 1.f;
						fGameStartTimer -= dt;
						if (fGameStartTimer <= 0.f)
						{
							fGameStartTimer = 0.f;
							ChangeState(G_MAINMENU);
						}
					}
				}
			}
		}
		return;
	}
		break;
	case G_MAINMENU:
	{
		static bool bLButtonState = false;
		if (!bLButtonState && Application::IsMousePressed(0))
		{
			bLButtonState = true;
		}
		else if (bLButtonState && !Application::IsMousePressed(0))
		{
			bLButtonState = false;
			ChangeState(G_INPLAY);
		}
		return;
	}
		break;
	default:
	{

	}
		break;
	}

	camera.Update(dt);

	if (KC->IsKeyPressed('P')) {//A TEST TO CHANGE RELIGION VALUE DIS WONT BE IN DA FNIAL GAME
		CM->AddToCalamityQueue(new CalamityEarthquake());
		SD->SetReligionValue(((int)SD->GetReligionValue() % (int)SD->GetMaxReligionValue()) + 25);
	}
	if (Application::IsKeyPressed(VK_OEM_MINUS))
	{
		m_speed = Math::Max(0.f, m_speed - 0.1f);
	}
	if (Application::IsKeyPressed(VK_OEM_PLUS))
	{
		m_speed += 0.1f;
	}

	float LSPEED = 10.0f;
	if (Application::IsKeyPressed('I'))
		lights[0].position.z -= (float)(LSPEED * dt);
	if (Application::IsKeyPressed('K'))
		lights[0].position.z += (float)(LSPEED * dt);
	if (Application::IsKeyPressed('J'))
		lights[0].position.x -= (float)(LSPEED * dt);
	if (Application::IsKeyPressed('L'))
		lights[0].position.x += (float)(LSPEED * dt);
	if (Application::IsKeyPressed('N'))
		lights[0].position.y -= (float)(LSPEED * dt);
	if (Application::IsKeyPressed('M'))
		lights[0].position.y += (float)(LSPEED * dt);

	if (Application::IsKeyPressed('Z'))
		lights[0].type = Light::LIGHT_POINT;
	else if (Application::IsKeyPressed('X'))
		lights[0].type = Light::LIGHT_DIRECTIONAL;
	else if (Application::IsKeyPressed('C'))
		lights[0].type = Light::LIGHT_SPOT;

	//Input Section
	static bool bPState = false;
	if (!bPState && Application::IsKeyPressed('P'))
	{
		bPState = true;
	}
	else if (bPState && !Application::IsKeyPressed('P'))
	{
		bPState = false;
		std::cout << "P UP" << std::endl;
	}

	if (KC->IsKeyPressed('G'))
	{
		bShowGrid = !bShowGrid;
	}

	if (KC->IsKeyPressed('B'))
	{
		if (selected == NULL)
		{
			selected = FetchGO(GameObject::GO_BUILDING);
			GridPt currentGrid = GetPoint(mousePos);
			selected->pos = GetGridPos(currentGrid);
			selected->pos.y += selected->scale.y * 0.5f;
			static_cast<Building*>(selected)->eCurrState = Building::BLUEPRINT;

			bShowGrid = true;
		}
		else
		{
			Building* goBuilding = dynamic_cast<Building*>(selected);
			if (goBuilding)
			{
				//Only Completed and non broken buildings can be moved again
				if (goBuilding->eCurrState == Building::COMPLETED)
				{
					goBuilding->eCurrState = Building::BLUEPRINT;
					GridPt currentGrid = GetPoint(mousePos);
					selected->pos = GetGridPos(currentGrid);
					selected->pos.y += selected->scale.y * 0.5f;

					bShowGrid = true;
				}
				//Only non broken buildings can be moved again
				/*if (goBuilding->eCurrState != Building::BROKEN)
				{
					goBuilding->eCurrState = Building::BLUEPRINT;
					GridPt currentGrid = GetPoint(mousePos);
					selected->pos = GetGridPos(currentGrid);
					selected->pos.y += selected->scale.y * 0.5f;

					bShowGrid = true;
				}*/
			}
		}
	}

	if (KC->IsKeyPressed('F'))
	{
		if (selected != NULL)
		{
			if (selected == goAltar)
			{
				if (SD->GetFood() > 0)
				{
					//For now 1 food 10 food offered, change to 1 to 1 later
					static_cast<Altar*>(goAltar)->iFoodOffered += 10;
					SD->SetFood(SD->GetFood() - 1);
				}
			}
		}
	}

	Vector3 clickTarget = NULL;
	static bool bLButtonState = false;
	if (!bLButtonState && Application::IsMousePressed(0))
	{
		bLButtonState = true;
		//std::cout << "LBUTTON DOWN" << std::endl;

		double x, y;
		Application::GetCursorPos(&x, &y);
		int w = Application::GetWindowWidth();
		int h = Application::GetWindowHeight();
		float posX = static_cast<float>(x) / w * m_worldWidth;
		float posY = (h - static_cast<float>(y)) / h * m_worldHeight;
		//std::cout << mousePos << std::endl;
		GridPt selectedPt = GetPoint(mousePos);
		if (isPointInGrid(selectedPt))
			//std::cout << "Selected Grid: " << selectedPt.x << ", " << selectedPt.z << std::endl;
		{
			bool objectFound = false;
			for (auto go : m_goList)
			{
				if (!go->active)
					continue;
				if (selectedPt == go->currentPt)
				{
					//Supposed to implement priority here
					if (selected == NULL)
					{
						selected = go;
						objectFound = true;
						break;
					}
					else if (selected != go)
					{
						objectFound = true;
						switch (selected->type)
						{
						case GameObject::GO_VILLAGER:
						{
							selected->m_nextState = SMManager::GetInstance()->GetSM(selected->smID)->GetState("Idle");
							selected->goTarget = go;
							selected = NULL;
							objectFound = true;
						}
						break;
						default:
							break;
						}
					}
				}
				if (objectFound)
				{
					break;
				}
			}
			if (!objectFound)
			{
				if (selected != NULL)
				{
					switch (selected->type)
					{
					case GameObject::GO_VILLAGER:
					{
						selected->goTarget = NULL;
						if (m_grid[GetGridIndex(selectedPt)] == Grid::TILE_EMPTY)
						{
							selected->target = GetGridPos(selectedPt);
							selected->m_nextState = SMManager::GetInstance()->GetSM(selected->smID)->GetState("Idle");
						}
					}
					break;

					//Test with just GO_BUILDING first
					case GameObject::GO_BUILDING:
					{
						Building* goBuilding = static_cast<Building*>(selected);
						if (goBuilding->eCurrState == Building::BLUEPRINT)
						{
							//Should be trying to contruct now
							if(!goBuilding->bBuilt)
								goBuilding->eCurrState = Building::CONSTRUCTING;
							else
								goBuilding->eCurrState = Building::COMPLETED;
							bShowGrid = false;
						}
					}
					break;
					case GameObject::GO_CHIEFHUT:
					{
						//Chief Hut should be built in init
						Building* goBuilding = static_cast<Building*>(selected);
						if (goBuilding->eCurrState == Building::BLUEPRINT)
						{
							goBuilding->eCurrState = Building::COMPLETED;
							bShowGrid = false;
						}
					}
					break;
					}
					selected = NULL;
					goVillager->goTarget = NULL;
				}
			}
		}
	}
	else if (bLButtonState && !Application::IsMousePressed(0))
	{
		bLButtonState = false;
		//std::cout << "LBUTTON UP" << std::endl;
	}

	// day/night cycle
	fTimeOfDay += dt * m_speed;
	if (fTimeOfDay >= 24.f)
		fTimeOfDay = 0;
	else if (fTimeOfDay >= 6.f && fTimeOfDay <= 18.f && !bDay) // 0600 to 1800 day
		ChangeTimeOfDay();
	else if ((fTimeOfDay <= 6.f || fTimeOfDay >= 18.f) && bDay) // 1800 to 0600 night
		ChangeTimeOfDay();
	float SHADOW_LENGTH = 1.f;
	if (bDay)
	{
		lights[0].position.x = (12.f - fTimeOfDay) * SHADOW_LENGTH;
		lights[0].position.y = (-0.5f * pow(lights[0].position.x,2)) + 10;
	}
	else if(fTimeOfDay >= 18.f)
	{
		//lights[0].position.x = (fTimeOfDay - 24.f) * SHADOW_LENGTH;
	}
	else if(fTimeOfDay <= 6.f)
	{
		//lights[0].position.x = fTimeOfDay * SHADOW_LENGTH;
	}
	// month
	if (SD->GetCurrDay() >= 31)
	{
		SD->SetCurrDay(1);
		ProgressMonth();
	}

	// sea movement
	if (fSeaDeltaX >= SEA_WIDTH / 4)
		fSeaDeltaX = -SEA_WIDTH / 4;
	fSeaDeltaX += dt * m_speed;
	if (fSeaDeltaZ >= SEA_HEIGHT / 4)
		fSeaDeltaZ = -SEA_HEIGHT / 4;
	fSeaDeltaZ += dt * m_speed;

	fSeaDeltaY = 0.0625f + 0.0625f * cosf(SD->GetElapsedTime());

	//goals
	switch (SD->GetCurrMonth())
	{
	case 1:
		bGoalAchieved = SD->GetReligionValue() >= SD->GetMaxReligionValue();
		break;
	}


	ProjectileManager::GetInstance()->Update(dt * m_speed);

	//There is a currently selected object
	if (selected != NULL)
	{
		Building* goBuilding = dynamic_cast<Building*>(selected);
		if (goBuilding) //selected is of a building class
		{
			if (goBuilding->eCurrState == Building::BLUEPRINT) //Building editor mode
			{
				GridPt currentGrid = GetPoint(mousePos);
				selected->pos = GetGridPos(currentGrid);
				selected->pos.y += selected->scale.y * 0.5f;
			}
			else if(goBuilding->eCurrState != Building::CONSTRUCTING)//The building is either completed / half broken
			{
				//Pop up some ui or something, maybe somewhere else
			}
		}
	}
	//Update the Grid
	std::fill(m_grid.begin(), m_grid.end(), Grid::TILE_EMPTY);
	m_grid[5] = Grid::TILE_USED;
	m_grid[5 + SD->GetNoGrid() * 1] = Grid::TILE_USED;
	m_grid[5 + SD->GetNoGrid() * 2] = Grid::TILE_USED;
	m_grid[5 + SD->GetNoGrid() * 3] = Grid::TILE_USED;
	m_grid[5 + SD->GetNoGrid() * 4] = Grid::TILE_USED;
	for (auto go : m_goList)
	{
		if (!go->active)
			continue;
		GridPt gridPt = go->currentPt;
		if (isPointInGrid(gridPt))
		{
			Vector3 ptPos = GetGridPos(gridPt); //Position of gridPt in world space

			//Check if this GameObject takes up a GridPt
			if (go->iGridX > 0 && go->iGridZ > 0)
			{
				//Since this value is rounded down, this value is for the closer side of GridPt, refer to gridImage in beside solution file
				int iHalfGridX = go->iGridX * 0.5f;
				int iHalfGridZ = go->iGridZ * 0.5f;

				//Check which side
				bool bLeftSide = false;
				if (go->pos.x < ptPos.x)
				{
					bLeftSide = true;
				}
				//Check which side
				bool bUpSide = false;
				if (go->pos.z < ptPos.z)
				{
					bUpSide = true;
				}

				//x-axis
				for (int i = 1; i <= iHalfGridX; ++i)
				{
					for (int j = 1; j <= iHalfGridZ; ++j)
					{
						if (bLeftSide)
						{
							if (bUpSide)
							{
								GridPt pointToChange(gridPt.x - i, gridPt.z - j);
								if (isPointInGrid(pointToChange))
								{
									if (m_grid[GetGridIndex(pointToChange)] == Grid::TILE_EMPTY)
									{
										m_grid[GetGridIndex(pointToChange)] = Grid::TILE_USED;
									}
								}
							}
							else
							{
								GridPt pointToChange(gridPt.x - i, gridPt.z + j);
								if (isPointInGrid(pointToChange))
								{
									if (m_grid[GetGridIndex(pointToChange)] == Grid::TILE_EMPTY)
									{
										m_grid[GetGridIndex(pointToChange)] = Grid::TILE_USED;
									}
								}
							}
						}
						else
						{
							if (bUpSide)
							{
								GridPt pointToChange(gridPt.x + i, gridPt.z - j);
								if (isPointInGrid(pointToChange))
								{
									if (m_grid[GetGridIndex(pointToChange)] == Grid::TILE_EMPTY)
									{
										m_grid[GetGridIndex(pointToChange)] = Grid::TILE_USED;
									}
								}
							}
							else
							{
								GridPt pointToChange(gridPt.x + i, gridPt.z + j);
								if (isPointInGrid(pointToChange))
								{
									if (m_grid[GetGridIndex(pointToChange)] == Grid::TILE_EMPTY)
									{
										m_grid[GetGridIndex(pointToChange)] = Grid::TILE_USED;
									}
								}
							}
						}
					}
					for (int j = 0; j < (go->iGridZ - iHalfGridZ); ++j)
					{
						if (bLeftSide)
						{
							if (bUpSide)
							{
								GridPt pointToChange(gridPt.x - i, gridPt.z + j);
								if (isPointInGrid(pointToChange))
								{
									if (m_grid[GetGridIndex(pointToChange)] == Grid::TILE_EMPTY)
									{
										m_grid[GetGridIndex(pointToChange)] = Grid::TILE_USED;
									}
								}
							}
							else
							{
								GridPt pointToChange(gridPt.x - i, gridPt.z - j);
								if (isPointInGrid(pointToChange))
								{
									if (m_grid[GetGridIndex(pointToChange)] == Grid::TILE_EMPTY)
									{
										m_grid[GetGridIndex(pointToChange)] = Grid::TILE_USED;
									}
								}
							}
						}
						else
						{
							if (bUpSide)
							{
								GridPt pointToChange(gridPt.x + i, gridPt.z + j);
								if (isPointInGrid(pointToChange))
								{
									if (m_grid[GetGridIndex(pointToChange)] == Grid::TILE_EMPTY)
									{
										m_grid[GetGridIndex(pointToChange)] = Grid::TILE_USED;
									}
								}
							}
							else
							{
								GridPt pointToChange(gridPt.x + i, gridPt.z - j);
								if (isPointInGrid(pointToChange))
								{
									if (m_grid[GetGridIndex(pointToChange)] == Grid::TILE_EMPTY)
									{
										m_grid[GetGridIndex(pointToChange)] = Grid::TILE_USED;
									}
								}
							}
						}
					}
				}
				for (int i = 0; i < (go->iGridX - iHalfGridX); ++i)
				{
					for (int j = 1; j <= iHalfGridZ; ++j)
					{
						if (bLeftSide)
						{
							if (bUpSide)
							{
								GridPt pointToChange(gridPt.x + i, gridPt.z - j);
								if (isPointInGrid(pointToChange))
								{
									if (m_grid[GetGridIndex(pointToChange)] == Grid::TILE_EMPTY)
									{
										m_grid[GetGridIndex(pointToChange)] = Grid::TILE_USED;
									}
								}
							}
							else
							{
								GridPt pointToChange(gridPt.x + i, gridPt.z + j);
								if (isPointInGrid(pointToChange))
								{
									if (m_grid[GetGridIndex(pointToChange)] == Grid::TILE_EMPTY)
									{
										m_grid[GetGridIndex(pointToChange)] = Grid::TILE_USED;
									}
								}
							}
						}
						else
						{
							if (bUpSide)
							{
								GridPt pointToChange(gridPt.x - i, gridPt.z - j);
								if (isPointInGrid(pointToChange))
								{
									if (m_grid[GetGridIndex(pointToChange)] == Grid::TILE_EMPTY)
									{
										m_grid[GetGridIndex(pointToChange)] = Grid::TILE_USED;
									}
								}
							}
							else
							{
								GridPt pointToChange(gridPt.x - i, gridPt.z + j);
								if (isPointInGrid(pointToChange))
								{
									if (m_grid[GetGridIndex(pointToChange)] == Grid::TILE_EMPTY)
									{
										m_grid[GetGridIndex(pointToChange)] = Grid::TILE_USED;
									}
								}
							}
						}
					}
					for (int j = 0; j < (go->iGridZ - iHalfGridZ); ++j)
					{
						if (bLeftSide)
						{
							if (bUpSide)
							{
								GridPt pointToChange(gridPt.x + i, gridPt.z + j);
								if (isPointInGrid(pointToChange))
								{
									if (m_grid[GetGridIndex(pointToChange)] == Grid::TILE_EMPTY)
									{
										m_grid[GetGridIndex(pointToChange)] = Grid::TILE_USED;
									}
								}
							}
							else
							{
								GridPt pointToChange(gridPt.x + i, gridPt.z - j);
								if (isPointInGrid(pointToChange))
								{
									if (m_grid[GetGridIndex(pointToChange)] == Grid::TILE_EMPTY)
									{
										m_grid[GetGridIndex(pointToChange)] = Grid::TILE_USED;
									}
								}
							}
						}
						else
						{
							if (bUpSide)
							{
								GridPt pointToChange(gridPt.x - i, gridPt.z + j);
								if (isPointInGrid(pointToChange))
								{
									if (m_grid[GetGridIndex(pointToChange)] == Grid::TILE_EMPTY)
									{
										m_grid[GetGridIndex(pointToChange)] = Grid::TILE_USED;
									}
								}
							}
							else
							{
								GridPt pointToChange(gridPt.x - i, gridPt.z - j);
								if (isPointInGrid(pointToChange))
								{
									if (m_grid[GetGridIndex(pointToChange)] == Grid::TILE_EMPTY)
									{
										m_grid[GetGridIndex(pointToChange)] = Grid::TILE_USED;
									}
								}
							}
						}
					}
				}
			}
		}
	}

	//Update the GameObjects
	static const float NPC_VELOCITY = 10.f;
	SD->SetPopulation(0);
	SD->SetPopulationLimit(0);
	for (auto go : m_goList)
	{
		if (!go->active)
			continue;
		go->currentPt = GetPoint(go->pos);
		// updating GameObjects
		switch (go->type)
		{
		case GameObject::GO_VILLAGER:
			SD->SetPopulation(SD->GetPopulation() + 1);
			break;
		case GameObject::GO_CHIEFHUT:
			SD->SetPopulationLimit(SD->GetPopulationLimit() + 10);
			break;
		case GameObject::GO_BUSH:
			break;
		case GameObject::GO_ALTAR:
			SD->SetReligionValue(Math::Min(100.f, (float)(static_cast<Altar*>(go)->iFoodOffered)));
			break;
		default:
			break;
		}


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
	switch (go->type)
	{
	case GameObject::GO_VILLAGER:
	{
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		//Offset to center to middle of used grids
		if (go->iGridX % 2 == 0)
		{
			modelStack.Translate(SceneData::GetInstance()->GetGridSize() * 0.5f, 0, 0);
		}
		if (go->iGridZ % 2 == 0)
		{
			modelStack.Translate(0, 0, SceneData::GetInstance()->GetGridSize() * 0.5f);
		}
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		float angle = Math::RadianToDegree(atan2(-go->direction.z, go->direction.x));
		modelStack.Rotate(angle, 0, 1, 0);
		//std::cout << angle << std::endl;
		//RenderMesh(meshList[GEO_VILLAGER], false, 1.f);
		//RenderMesh(meshList[GEO_TREE], false, 1.f);
		//RenderMesh(meshList[GEO_BERRIES], false, 1.f);
		//RenderMesh(meshList[GEO_BUSH], false, 1.f);
		//RenderMesh(meshList[GEO_VILLAGER], bGodlights, 1.f);
		modelStack.Rotate(-90, 1, 0, 0);
		RenderMesh(meshList[GEO_RED_CASTER], bGodlights, 1.f);
		modelStack.PopMatrix();
	}
	break;
	case GameObject::GO_BUILDING:
	{
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		//Current state of the building
		switch (static_cast<Building*>(go)->eCurrState)
		{
		case Building::COMPLETED:
			RenderMesh(meshList[GEO_BUILDING], bGodlights, 1.f);
			break;
		case Building::BLUEPRINT:
			RenderMesh(meshList[GEO_BUILDING], bGodlights, 0.2f);
			break;
		case Building::CONSTRUCTING:
			RenderMesh(meshList[GEO_BUILDING], bGodlights, 0.6f);
			break;
		case Building ::BROKEN:
			RenderMesh(meshList[GEO_BROKEN_BUILDING], bGodlights, 1.f);
		}
		modelStack.PopMatrix();
	}
	break;
	case GameObject::GO_CHIEFHUT:
	{
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_BUILDING], bGodlights, 1.f);
		modelStack.PopMatrix();
	}
	break;
	case GameObject::GO_ALTAR:
	{
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_BUILDING], bGodlights, 1.f);
		modelStack.PopMatrix();
	}
	break;
	case GameObject::GO_BUSH:
	{
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_BUSH], bGodlights, 1.f);
		if (static_cast<Bush*>(go)->eCurrState == Bush::LUSH)
			RenderMesh(meshList[GEO_BERRIES], bGodlights, 1.f);
		modelStack.PopMatrix();
	}
	break;
	case GameObject::GO_TREE:
	{
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		//Offset to center to middle of used grids
		if (go->iGridX % 2 == 0)
		{
			modelStack.Translate(SceneData::GetInstance()->GetGridSize() * 0.5f, 0, 0);
		}
		if (go->iGridZ % 2 == 0)
		{
			modelStack.Translate(0, 0, SceneData::GetInstance()->GetGridSize() * 0.5f);
		}
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		Tree* goTree = static_cast<Tree*>(go);
		switch (goTree->eCurrState)
		{
		case Tree::FULL:
			RenderMesh(meshList[GEO_TREE], bGodlights, 1.f);
			break;
		case Tree::HALFCHOPPED:
			RenderMesh(meshList[GEO_HALFTREE], bGodlights, 1.f);
			break;
		default:
			RenderMesh(meshList[GEO_TREE], bGodlights, 1.f);
			break;
		}
		modelStack.PopMatrix();
	}
	break;
	default:
		break;
	}
}
void SceneSP::RenderPassGPass()
{
	m_renderPass = RENDER_PASS_PRE;
	m_lightDepthFBO.BindForWriting();
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glClear(GL_DEPTH_BUFFER_BIT);
	glUseProgram(m_gPassShaderID);
	if (lights[0].type == Light::LIGHT_DIRECTIONAL)
		m_lightDepthProj.SetToOrtho(-64, 64, -64, 64, -64, 64);
	else
		m_lightDepthProj.SetToPerspective(90, 1.f, 0.1, 1000);
	m_lightDepthView.SetToLookAt(lights[0].position.x,
		lights[0].position.y, lights[0].position.z, 0, 0, 0, 0, 1, 0);
	RenderWorld();
}

void SceneSP::RenderPassMain()
{
	m_renderPass = RENDER_PASS_MAIN;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, Application::GetWindowWidth(),
		Application::GetWindowHeight());
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(m_programID);
	//pass light depth texture
	m_lightDepthFBO.BindForReading(GL_TEXTURE8);
	glUniform1i(m_parameters[U_SHADOW_MAP], 8);

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

	if (lights[0].type == Light::LIGHT_DIRECTIONAL)
	{
		Vector3 lightDir(lights[0].position.x, lights[0].position.y, lights[0].position.z);
		Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightDirection_cameraspace.x);
	}
	else if (lights[0].type == Light::LIGHT_SPOT)
	{
		Position lightPosition_cameraspace = viewStack.Top() * lights[0].position;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);
		Vector3 spotDirection_cameraspace = viewStack.Top() * lights[0].spotDirection;
		glUniform3fv(m_parameters[U_LIGHT0_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
	}
	else
	{
		Position lightPosition_cameraspace = viewStack.Top() * lights[0].position;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);
	}

	modelStack.PushMatrix();
	modelStack.Translate(lights[0].position.x, lights[0].position.y, lights[0].position.z);
	//RenderMesh(meshList[GEO_BALL], false);
	modelStack.PopMatrix();

	//modelStack.PushMatrix();
	//modelStack.Translate(mousePos.x, mousePos.y, mousePos.z);
	////modelStack.Scale(0.1, 0.1, 0.1);
	//RenderMesh(meshList[GEO_HAND_DEFAULT], true);
	//modelStack.PopMatrix();

	//RenderMesh(meshList[GEO_AXES], false);

	modelStack.PushMatrix();
	//modelStack.Translate(0, 0.5f + cosf(asd) * 0.15f, 0);
	modelStack.Translate(0, -0.5f, 0);
	//modelStack.Translate(0, 0, 0);
	//modelStack.Translate(0, -0.5f, 0);
	//modelStack.Rotate(-90, 1, 0, 0);
	//modelStack.Scale(15, 15, 15);
	modelStack.Scale(5, 1, 5);
	RenderMesh(meshList[GEO_ISLAND], bGodlights);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(fSeaDeltaX, fSeaDeltaY - 0.51f, fSeaDeltaZ);
	modelStack.Rotate(-90, 1, 0, 0);
	modelStack.Scale(SEA_WIDTH, SEA_HEIGHT, SEA_HEIGHT);
	RenderMesh(meshList[GEO_SEA], bGodlights, 0.75f);
	modelStack.PopMatrix();

	RenderWorld();

	modelStack.PushMatrix();
	modelStack.Translate(0, 2, 0);
	modelStack.Scale(5, 5, 5);
	//RenderMesh(meshList[GEO_LIGHT_DEPTH_QUAD], false);
	modelStack.PopMatrix();

	SceneData* SD = SceneData::GetInstance();

	//Render Grid
	if (bShowGrid)
	{
		modelStack.PushMatrix();
		modelStack.Translate(-0.5f * SD->GetNoGrid() * SD->GetGridSize(), 0.5f, -0.5f * SD->GetNoGrid() * SD->GetGridSize());
		modelStack.Scale(1, 1, 1);
		glLineWidth(2.f);
		RenderMesh(meshList[GEO_GRID], false);
		glLineWidth(1.f);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(-0.5f * SD->GetNoGrid() * SD->GetGridSize(), 0.5f, -0.5f * SD->GetNoGrid() * SD->GetGridSize());
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

		GridPt selectedGrid = GetPoint(mousePos);
		if (isPointInGrid(selectedGrid))
		{
			Vector3 gridPos = GetGridPos(selectedGrid);
			modelStack.PushMatrix();
			modelStack.Translate(gridPos.x, gridPos.y + 0.5f, gridPos.z);
			modelStack.Rotate(-90, 1, 0, 0);
			modelStack.Scale(SD->GetGridSize(), SD->GetGridSize(), SD->GetGridSize());
			RenderMesh(meshList[GEO_YELLOWQUAD], false, 0.4f);
			modelStack.PopMatrix();
		}
	}

	if (selected != NULL)
	{
		modelStack.PushMatrix();
		modelStack.Translate(selected->pos.x, selected->pos.y + selected->scale.y * 0.7f, selected->pos.z);
		modelStack.Scale(0.1, 0.1, 0.1);
		RenderMesh(meshList[GEO_VILLAGER], false); // renders a red cube above GO if it is currently selected
		modelStack.PopMatrix();
	}

	UIManager::GetInstance()->Render(this);

	//On screen text
	std::ostringstream ss;

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

	// resources x=10
	ss.str("");
	ss << "Wood:" << SD->GetWood() << "/" << SD->GetWoodLimit();
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 10, 6);

	ss.str("");
	ss << "Food:" << SD->GetFood() << "/" << SD->GetFoodLimit();
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 10, 3);

	ss.str("");
	ss << "Population:" << SD->GetPopulation() << "/" << SD->GetPopulationLimit();
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 10, 0);

	//time and month x=30
	ss.str("");
	ss << "Date: ";
	switch (SD->GetCurrMonth())
	{
	case 1:
		ss << "JAN";
		break;
	case 2:
		ss << "FEB";
		break;
	case 3:
		ss << "MAR";
		break;
	case 4:
		ss << "APR";
		break;
	case 5:
		ss << "MAY";
		break;
	case 6:
		ss << "JUN";

		break;
	case 7:
		ss << "JUL";
		break;
	case 8:
		ss << "AUG";
		break;
	case 9:
		ss << "SEP";
		break;
	case 10:
		ss << "OCT";
		break;
	case 11:
		ss << "NOV";
		break;
	case 12:
		ss << "DEC";
		break;
	}
	ss << " ";
	ss << SD->GetCurrDay();
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 30, 3);

	ss.str("");
	ss << "Time: ";
	float fractpart, intpart;
	fractpart = modf(fTimeOfDay, &intpart);
	if (intpart < 10)
		ss << "0" << intpart;
	else
		ss << intpart;
	ss << ":";
	if ((int)(60 * fractpart) < 10)
		ss << "0" << (int)(60 * fractpart);
	else
		ss << (int)(60 * fractpart);
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 30, 0);

	// objective
	ss.str("");
	ss << "Current Goal:";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 30);
	ss.str("");
	if (bGoalAchieved) 
		ss << "ACHIEVED";
	else
		ss << "Fill the Religion Meter by offering food to the altar.";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 27);
}

void SceneSP::RenderSplashScreen()
{
	m_renderPass = RENDER_PASS_MAIN;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, Application::GetWindowWidth(),
		Application::GetWindowHeight());
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(m_programID);
	//pass light depth texture
	m_lightDepthFBO.BindForReading(GL_TEXTURE8);
	glUniform1i(m_parameters[U_SHADOW_MAP], 8);

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

	//RenderMesh(meshList[GEO_AXES], false);


	if (fOpenGLOutTimer > 0.f)
	{
		modelStack.PushMatrix();
		modelStack.Translate(m_worldWidth * 0.5f, m_worldHeight * 0.5f, 1.f);
		modelStack.Scale(m_worldWidth, ((m_worldWidth / 1024) * 720), m_worldHeight);
		if (fOpenGLInTimer < 1.0f)
			RenderMesh(meshList[GEO_SPLASHSCREEN], false, fOpenGLInTimer);
		else
			RenderMesh(meshList[GEO_SPLASHSCREEN], false, fOpenGLOutTimer);
		modelStack.PopMatrix();
	}
	else
	{
		modelStack.PushMatrix();
		modelStack.Translate(m_worldWidth * 0.5f, m_worldHeight * 0.5f, 1.f);
		modelStack.Scale(m_worldWidth, ((m_worldWidth / 1024) * 720), m_worldHeight);
		if (fSplashScreenInTimer < 1.0f)
			RenderMesh(meshList[GEO_LOGO], false, fSplashScreenInTimer);
		else
			RenderMesh(meshList[GEO_LOGO], false, fSplashScreenOutTimer);
		modelStack.PopMatrix();
	}
}

void SceneSP::RenderMainMenu()
{
	m_renderPass = RENDER_PASS_MAIN;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, Application::GetWindowWidth(),
		Application::GetWindowHeight());
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(m_programID);
	//pass light depth texture
	m_lightDepthFBO.BindForReading(GL_TEXTURE8);
	glUniform1i(m_parameters[U_SHADOW_MAP], 8);

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

	//RenderMesh(meshList[GEO_AXES], false);
	
	modelStack.PushMatrix();
	modelStack.Translate(m_worldWidth * 0.5f, m_worldHeight * 0.5f, 0.f);
	modelStack.Scale(m_worldWidth, ((m_worldWidth / 1024) * 720), m_worldHeight);
	RenderMesh(meshList[GEO_LOGO], false);
	modelStack.PopMatrix();

	//buttonz
	modelStack.PushMatrix();
	modelStack.Translate(m_worldWidth * 0.8f, m_worldHeight * 0.7f, 1.f);
	modelStack.Scale(m_worldWidth*0.2f, m_worldHeight*0.1f, m_worldHeight);
	RenderMesh(meshList[GEO_WHITEQUAD], false);
	modelStack.PopMatrix();

	UIManager::GetInstance()->Render(this);
}

void SceneSP::RenderWorld()
{
	for (auto go : m_goList)
	{
		if (!go->active)
			continue;
		RenderGO(go);
	}
	EffectManager::GetInstance()->Render(this);
}

void SceneSP::Render()
{
	//SceneBase::Render();
	switch (game_state)
	{
	case G_SPLASHSCREEN:
		RenderSplashScreen();
		break;
	case G_MAINMENU:
		RenderMainMenu();
		break;
	case G_INPLAY:
		RenderPassGPass();
		RenderPassMain();
		break;
	default:
		break;
	}
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