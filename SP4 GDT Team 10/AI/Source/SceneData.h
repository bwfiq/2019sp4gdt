#ifndef SCENE_DATA
#define SCENE_DATA

#include "SingletonTemplate.h"
#include <map>
#include <string>

class Mesh;

class SceneData : public Singleton<SceneData>
{
	friend Singleton<SceneData>;
public:
	int GetObjectCount();
	void SetObjectCount(int objectCount);
	int GetFishCount();
	void SetFishCount(int fishCount);
	int GetNoGrid();
	void SetNoGrid(int noGrid);
	float GetGridSize();
	void SetGridSize(float gridSize);
	float GetGridOffset();
	void SetGridOffset(float gridOffset);

	float GetWorldHeight();
	void SetWorldHeight(float worldheight);
	float GetWorldWidth();
	void SetWorldWidth(float worldwidth);
	float GetElapsedTime();
	void SetElapsedTime(float elapsedTime);

	void AddMesh(Mesh* mesh);
	void RemoveMesh(const std::string& meshName);
	Mesh* GetMesh(const std::string& meshName);
private:
	SceneData();
	~SceneData();
	int m_objectCount;
	int m_fishCount;

	int m_noGrid;
	float m_gridSize;
	float m_gridOffset;

	float m_worldHeight;
	float m_worldWidth;
	float f_elapsedTime;

	int iFood, iFoodLimit, iPopulation, iPopulationLimit;


	std::map<std::string, Mesh*> meshList;//these meshes delete themselves via the scene
};

#endif
