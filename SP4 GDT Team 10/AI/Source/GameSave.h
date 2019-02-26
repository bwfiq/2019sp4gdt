#ifndef GAME_SAVE
#define GAME_SAVE

#include <string>
#include <vector>

#include <iostream>
#include <fstream>

//Json file stuff
#include <cstdlib>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/rapidjson.h>

class GameSave
{
public:
	GameSave();
	~GameSave();

	std::string filename;

	void SaveGame();

	template<typename T>
	void SaveData(T) {
		return 0;
	}
};
#endif