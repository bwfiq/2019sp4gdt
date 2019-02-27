#include "SceneData.h"
#include "Mesh.h"
#include <string>

static const std::string months[] = {
	"NONE"//since it starts at 0
	,"JAN"
	,"FEB"
	,"MAR"
	,"APR"
	,"MAY"
	,"JUN"
	,"JUL"
	,"AUG"
	,"SEP"
	,"OCT"
	,"NOV"
	,"DEC"
};

int SceneData::GetObjectCount()
{
	return m_objectCount;
}

void SceneData::SetObjectCount(int objectCount)
{
	m_objectCount = objectCount;
}

int SceneData::GetFishCount()
{
	return m_fishCount;
}

void SceneData::SetFishCount(int fishCount)
{
	m_fishCount = fishCount;
}

int SceneData::GetNoGrid()
{
	return m_noGrid;
}

void SceneData::SetNoGrid(int noGrid)
{
	m_noGrid = noGrid;
}

float SceneData::GetGridSize()
{
	return m_gridSize;
}

void SceneData::SetGridSize(float gridSize)
{
	m_gridSize = gridSize;
}

float SceneData::GetGridOffset()
{
	return m_gridOffset;
}

void SceneData::SetGridOffset(float gridOffset)
{
	m_gridOffset = gridOffset;
}

float SceneData::GetWorldHeight()
{
	return m_worldHeight;
}

void SceneData::SetWorldHeight(float worldheight)
{
	m_worldHeight = worldheight;
}

float SceneData::GetWorldWidth()
{
	return m_worldWidth;
}

void SceneData::SetWorldWidth(float worldwidth)
{
	m_worldWidth = worldwidth;
}

float SceneData::GetElapsedTime()
{
	return this->f_elapsedTime;
}

void SceneData::SetElapsedTime(float elapsedTime)
{
	this->f_elapsedTime = elapsedTime;
}

float SceneData::GetReligionValue()
{
	return this->fReligionValue;
}
void SceneData::SetReligionValue(float religionValue)
{
	this->fReligionValue = religionValue;
}

float SceneData::GetMaxReligionValue()
{
	return this->fMaxReligionValue;
}

void SceneData::SetMaxReligionValue(float maxreligionValue)
{
	this->fMaxReligionValue = maxreligionValue;
}

void SceneData::SetFood(int food)
{
	this->iFood = food;
}
int SceneData::GetFood()
{
	return iFood;
}
void SceneData::SetWood(int wood)
{
	this->iWood = wood;
}
int SceneData::GetWood()
{
	return iWood;
}
void SceneData::SetStone(int stone)
{
	this->iStone = stone;
}
int SceneData::GetStone()
{
	return iStone;
}
void SceneData::SetFoodLimit(int foodLimit)
{
	this->iFoodLimit = foodLimit;
}
int SceneData::GetFoodLimit()
{
	return iFoodLimit;
}
void SceneData::SetWoodLimit(int woodLimit)
{
	this->iWoodLimit = woodLimit;
}
int SceneData::GetWoodLimit()
{
	return iWoodLimit;
}
void SceneData::SetStoneLimit(int stoneLimit)
{
	this->iStoneLimit = stoneLimit;
}
int SceneData::GetStoneLimit()
{
	return iStoneLimit;
}
void SceneData::SetPopulation(int population)
{
	this->iPopulation = population;
}
int SceneData::GetPopulation()
{
	return iPopulation;
}
void SceneData::SetPopulationLimit(int populationLimit)
{
	this->iPopulationLimit = populationLimit;
}
int SceneData::GetPopulationLimit()
{
	return iPopulationLimit;
}

void SceneData::SetCurrMonth(int month)
{
	this->iCurrMonth = month;
}

int SceneData::GetCurrMonth()
{
	return iCurrMonth;
}

std::string SceneData::GetCurrMonth_string()
{
	return months[GetCurrMonth()];
}

void SceneData::SetCurrDay(int day)
{
	this->iCurrDay = day;
}

int SceneData::GetCurrDay()
{
	return iCurrDay;
}

void SceneData::SetMousePos_World(Vector3 mousePos_world)
{
	this->mousePos_World = mousePos_world;
}

void SceneData::SetCamPosition(Vector3 camPos)
{
	camPosition = camPos;
}

void SceneData::SetCamTarget(Vector3 camTarget)
{
	this->camTarget = camTarget;
}

Vector3 SceneData::GetMousePos_World()
{
	return this->mousePos_World;
}

Vector3 SceneData::GetCamPosition()
{
	return camPosition;
}

Vector3 SceneData::GetCamTarget()
{
	return this->camTarget;
}

void SceneData::SetMainMenuElapsedTime(float time)
{
	this->f_mainmenuElapsedTime = time;
}

float SceneData::GetMainMenuElapsedTime()
{
	return this->f_mainmenuElapsedTime;
}

void SceneData::SetTimeOfDay(float timeOfDay)
{
	this->fTimeOfDay = timeOfDay;
}

float SceneData::GetTimeOfDay()
{
	return this->fTimeOfDay;
}

std::string SceneData::GetTimeOfDay_string()
{
	float fractpart, intpart;
	fractpart = modf(fTimeOfDay, &intpart);
	std::string tod;
	if (intpart < 10)
		tod += "0" + std::to_string((int)intpart);
	else
		tod += std::to_string((int)intpart);
	tod += ":";
	if ((int)(60 * fractpart) < 10)
		tod += "0" + std::to_string((int)(60 * fractpart));
	else
		tod += std::to_string((int)(60 * fractpart));
	return tod;
}

void SceneData::AddMesh(Mesh * mesh)
{
	if (mesh == nullptr || mesh == NULL)
		return;

	RemoveMesh(mesh->name);

	meshList[mesh->name] = mesh;
}
void SceneData::RemoveMesh(const std::string & meshName)
{
	Mesh* currMesh = GetMesh(meshName);
	if (currMesh != nullptr && currMesh != NULL)
	{
		delete currMesh;
		meshList.erase(meshName);
	}
}

Mesh * SceneData::GetMesh(const std::string & meshName)
{
	if (meshList.count(meshName) != 0)
		return meshList[meshName];

	return NULL;
}

std::vector<GameObject*>* SceneData::GetGOList()
{
	return this->m_goList;
}

void SceneData::SetGOList(std::vector<GameObject*>* goList)
{
	this->m_goList = goList;
}

SceneData::SceneData()
{
}

SceneData::~SceneData()
{
}
