#include "SceneData.h"
#include "Mesh.h"

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
	return this->f_religionValue;
}
void SceneData::SetReligionValue(float religionValue)
{
	this->f_religionValue = religionValue;
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

SceneData::SceneData()
{
}

SceneData::~SceneData()
{
}
