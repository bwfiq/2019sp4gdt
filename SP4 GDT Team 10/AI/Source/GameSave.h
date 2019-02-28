#ifndef GAME_SAVE
#define GAME_SAVE

#include <string>
#include <vector>

#include <iostream>
#include <fstream>
#include "GameObject.h"
#include "CalamityBase.h"

//Json file stuff
#include <cstdlib>
#include <cstdio>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/rapidjson.h>
#include <rapidjson/filewritestream.h>
#include <rapidjson/filereadstream.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/error/en.h>

class GameSave
{
public:
	GameSave();
	~GameSave();

	std::string filename;

	rapidjson::Document gameFile;

	bool LoadGame();
	void SaveGame();
	void ResetGame(); //Clean the game file with a default value

	rapidjson::Value SaveVector3(Vector3 vector);
	rapidjson::Value SaveGridPt(GridPt gridPt);
	rapidjson::Value SaveVillager(GameObject* go);
	rapidjson::Value SavePig(GameObject* go);
	rapidjson::Value SaveBuilding(GameObject* go);
	rapidjson::Value SaveEnvironment(GameObject* go);
	rapidjson::Value SaveCalamityGo(GameObject* go);
	rapidjson::Value SaveCalamity(CalamityBase* go);

	Vector3 LoadVector3(rapidjson::Value& vectorValue);
	GridPt LoadGridPt(rapidjson::Value& gridValue);
	GameObject* LoadVillager(rapidjson::Value& villgaerValue);
	GameObject* LoadBuilding(rapidjson::Value& buildingValue);
	GameObject* LoadEnvironment(rapidjson::Value& environmentValue);
	GameObject* LoadCalamityGo(rapidjson::Value& calamityGOValue);
	GameObject* LoadPig(rapidjson::Value& pigValue);
	CalamityBase* LoadCalamity(rapidjson::Value& calamityValue);

	template<typename T>
	void SaveData(T) {
		return 0;
	}
};
#endif