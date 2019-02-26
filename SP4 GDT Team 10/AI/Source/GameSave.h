#ifndef GAME_SAVE
#define GAME_SAVE

#include <string>
#include <vector>

#include <iostream>
#include <fstream>


class GameSave
{
public:
	GameSave();
	~GameSave();

	std::string filename;

	void LoadGame();
	void SaveGame();

	template<typename T>
	void SaveData(T) {
		return 0;
	}
};
#endif