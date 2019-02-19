#ifndef SCENE_DATA
#define SCENE_DATA

#include "SingletonTemplate.h"
#include "Vector3.h"
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
	void  SetWorldHeight(float worldheight);
	float GetWorldWidth();
	void  SetWorldWidth(float worldwidth);
	float GetElapsedTime();
	void  SetElapsedTime(float elapsedTime);
	float GetReligionValue();
	void  SetReligionValue(float religionValue);
	float GetMaxReligionValue();
	void  SetMaxReligionValue(float maxreligionValue);

	void SetFood(int food);
	int GetFood();
	void SetWood(int wood);
	int GetWood();
	void SetFoodLimit(int foodLimit);
	int GetFoodLimit();
	void SetWoodLimit(int woodLimit);
	int GetWoodLimit();
	void SetPopulation(int population);
	int GetPopulation();
	void SetPopulationLimit(int populationLimit);
	int GetPopulationLimit();
	void SetResearchPoints(int researchPoints);
	int GetResearchPoints();
	void SetCurrMonth(int month);
	int GetCurrMonth();
	std::string GetCurrMonth_string();
	void SetCurrDay(int day);
	int GetCurrDay();
	void SetMousePos_World(Vector3 mousePos_world);
	void SetCamPosition(Vector3 camPos);
	Vector3 GetMousePos_World();
	Vector3 GetCamPosition();

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
	std::map<std::string, Mesh*> meshList;//these meshes delete themselves via the scene
	Vector3 mousePos_World;
	Vector3 camPosition;

	float fReligionValue, fMaxReligionValue;
	int iFood, iFoodLimit, iPopulation, iPopulationLimit, iWood, iWoodLimit, iResearchPoints, iCurrMonth, iCurrDay;
};

#endif
