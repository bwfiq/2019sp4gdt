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
