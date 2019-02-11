#include "SceneData.h"

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

SceneData::SceneData()
{
}

SceneData::~SceneData()
{
}
