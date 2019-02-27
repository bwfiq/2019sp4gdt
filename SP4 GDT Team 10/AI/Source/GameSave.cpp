#define _CRT_SECURE_NO_WARNINGS

#include "GameSave.h"
#include "SceneData.h"
#include "Villager.h"
#include "Building.h"
#include "Altar.h"
#include "Environment.h"
#include "Altar.h"
#include "ChiefHut.h"
#include "Granary.h"
#include "House.h"
#include "Logs.h"
#include "ResearchLab.h"
#include "WoodShed.h"
#include "Bush.h"
#include "Mountain.h"
#include "Tree.h"
#include "Tsunami.h"
#include "Tornado.h"

#include "CalamityManager.h"
#include "CalamityEarthquake.h"
#include "CalamityBlizzard.h"
#include "CalamityTornado.h"
#include "CalamityTsunami.h"
#include "CalamityWorldEnd.h"

using namespace rapidjson;
GameSave::GameSave()
{
	filename = "output.json";
	gameFile.SetObject();
}

GameSave::~GameSave()
{
}

bool GameSave::LoadGame()
{
	struct stat statbuffer;

	if (stat(filename.c_str(), &statbuffer) != 0)
	{
		std::cout << "hi, file does not exist, rapidjson" << std::endl;
		return false;
	}
	else

	{
		FILE* fp = fopen(filename.c_str(), "rb"); // non-Windows use "r"

		char readBuffer[65536];
		FileReadStream is(fp, readBuffer, sizeof(readBuffer));

		gameFile.ParseStream(is);
		fclose(fp);
		if (gameFile.HasParseError())
		{
			std::cout << "parse error" << std::endl;
			ParseErrorCode errorCode = gameFile.GetParseError();
			std::cout << GetParseError_En(errorCode) << std::endl;
			return false;
		}
		StringBuffer buffer;
		PrettyWriter<StringBuffer> writer(buffer);
		gameFile.Accept(writer);
		std::cout << buffer.GetString() << std::endl;

		Vector3 temp;
		Value objVillagers(kObjectType);
		Value objBuildings(kObjectType);
		Value objEnvironments(kObjectType);
		Value objCalamityGO(kObjectType);
		Value objCalamities(kObjectType);
		
		if (gameFile.IsObject())
		{
			/*if (gameFile.HasMember("Villagers"))
			{
				objVillagers = gameFile.FindMember("Villagers");
			}
			else
			{
				std::cout << "Cannot find Villagers Data" << std::endl;
				return false;
			}
			if (gameFile.HasMember("Buildings"))
			{
				objBuildings = gameFile.FindMember("Buildings");
			}
			else
			{
				std::cout << "Cannot find Buildings Data" << std::endl;
				return false;
			}
			if (gameFile.HasMember("Environments"))
			{
				objEnvironments = gameFile.FindMember("Environments");
			}
			else
			{
				std::cout << "Cannot find Environments Data" << std::endl;
				return false;
			}
			if (gameFile.HasMember("CalamityGO"))
			{
				objCalamityGO = gameFile.FindMember("CalamityGO");
			}
			else
			{
				std::cout << "Cannot find CalamityGO Data" << std::endl;
				return false;
			}
			if (gameFile.HasMember("Calamities"))
			{
				objCalamities = gameFile.FindMember("Calamities");
			}
			else
			{
				std::cout << "Cannot find Calamities Data" << std::endl;
				return false;
			}*/
		}
		else
		{
			std::cout << "JSON file is empty" << std::endl;
			return false;
		}

		//Value obj = gameFile.Get

		//Vector3 temp = LoadVector3(gameFile["Villagers"]["Villager"]["Pos"]);
	}
	return true;
}
void GameSave::SaveGame()
{
	gameFile.SetObject();
	SceneData* SD = SceneData::GetInstance();
	CalamityManager* CM = CalamityManager::GetInstance();
	//Villager* temp = new Villager();
	const char json[] = " { \"hello\" : \"world\", \"t\" : true , \"f\" : false, \"n\": null, \"i\":123, \"pi\": 3.1416, \"a\":[1, 2, 3, 4] } ";
	
	Document::AllocatorType& allocator = gameFile.GetAllocator();

	std::vector<GameObject*>* m_goList = SD->GetGOList();

	//d.AddMember("version", 1, allocator);
	//d.AddMember("testId", 2, allocator);
	//d.AddMember("group", 3, allocator);
	//d.AddMember("order", 4, allocator);

	Value objVillagers(kObjectType);
	Value objBuildings(kObjectType);
	Value objEnvironments(kObjectType);
	Value objCalamityGO(kObjectType);
	Value objCalamities(kObjectType);

	int numVil = 0;
	int numBuilding = 0;
	int numEnvironment = 0;
	int numCalamitiesGO = 0;
	int numCalamities = 0;
	int numCalamitiesQ = 0;
	Value villagers(kArrayType);
	Value buildings(kArrayType);
	Value environments(kArrayType);
	Value calamitiesGO(kArrayType);
	Value calamities(kArrayType);
	Value calamitiesQ(kArrayType);
	
	for (auto go : *m_goList)
	{
		if (!go->active)
			continue;
		//Dynamic cast to find choose between objects

		Villager* goVil = dynamic_cast<Villager*>(go);
		if (goVil)
		{
			villagers.PushBack(SaveVillager(go), allocator);
			++numVil;

			continue;
		}
		Building* goBuilding = dynamic_cast<Building*>(go);
		if (goBuilding)
		{
			buildings.PushBack(SaveBuilding(go), allocator);
			++numBuilding;

			continue;
		}
		Environment* goEnvironment = dynamic_cast<Environment*>(go);
		if (goEnvironment)
		{
			environments.PushBack(SaveEnvironment(go), allocator);
			++numEnvironment;

			continue;
		}
		Tsunami* goTsunami = dynamic_cast<Tsunami*>(go);
		if (goTsunami)
		{
			calamitiesGO.PushBack(SaveCalamityGo(go), allocator);
			++numCalamitiesGO;

			continue;
		}
		Tornado* goTornado = dynamic_cast<Tornado*>(go);
		if (goTornado)
		{
			calamitiesGO.PushBack(SaveCalamityGo(go), allocator);
			++numCalamitiesGO;

			continue;
		}
	}
	if (CM->GetCurrCalamity() != NULL)
	{
		calamities.PushBack(SaveCalamity(CM->GetCurrCalamity()), allocator);
		++numCalamities;
	}
	std::queue<CalamityBase*> calamityQueue = CM->GetCalamityQueue();
	while (!calamityQueue.empty())
	{
		++numCalamitiesQ;
		calamitiesQ.PushBack(SaveCalamity(calamityQueue.front()), allocator);
		calamityQueue.pop();
	}

	objVillagers.AddMember("Number", numVil, allocator);
	objVillagers.AddMember("Villager", villagers, allocator);

	objBuildings.AddMember("Number", numBuilding, allocator);
	objBuildings.AddMember("Building", buildings, allocator);

	objEnvironments.AddMember("Number", numEnvironment, allocator);
	objEnvironments.AddMember("Environment", environments, allocator);

	objCalamityGO.AddMember("Number", numCalamitiesGO, allocator);
	objCalamityGO.AddMember("Calamities GO", calamitiesGO, allocator);

	objCalamities.AddMember("Number", numCalamities, allocator);
	objCalamities.AddMember("Calamities", calamities, allocator);

	objCalamities.AddMember("Number Queue", numCalamitiesQ, allocator);
	objCalamities.AddMember("CalamitiesQ", calamitiesQ, allocator);

	gameFile.AddMember("Villagers", objVillagers, allocator);
	gameFile.AddMember("Buildings", objBuildings, allocator);
	gameFile.AddMember("Environments", objEnvironments, allocator);
	gameFile.AddMember("CalamityGO", objCalamityGO, allocator);
	gameFile.AddMember("Calamities", objCalamities, allocator);

	//Value tests(kArrayType);
	//Value obj(kObjectType);
	//Value val(kObjectType);

	//obj.AddMember("id", 1, allocator);

	//std::string description = "a description";
	//val.SetString(description.c_str(), static_cast<SizeType>(description.length()), allocator);
	//obj.AddMember("description", val, allocator);

	//std::string help = "some help";
	//val.SetString(help.c_str(), static_cast<SizeType>(help.length()), allocator);
	//obj.AddMember("help", val, allocator);

	//std::string workgroup = "a workgroup";
	//val.SetString(workgroup.c_str(), static_cast<SizeType>(workgroup.length()), allocator);
	//obj.AddMember("workgroup", val, allocator);

	//val.SetBool(true);
	//obj.AddMember("online", val, allocator);

	//tests.PushBack(obj, allocator);
	//d.AddMember("tests", tests, allocator);

	// Convert JSON document to string
	StringBuffer sb;
	PrettyWriter<StringBuffer> pwriter(sb);
	gameFile.Accept(pwriter);
	std::cout << sb.GetString() << std::endl;
	//d.Parse(json);
	//Document::AllocatorType& allocator = d.GetAllocator();
	//Value tempValue("{ \"hello\" : \"world\", \"t\" : true , \"f\" : false, \"n\": null, \"i\":123, \"pi\": 3.1416, \"a\":[1, 2, 3, 4] }", allocator);
	//Value tempname("hello");
	////d.PushBack(tempValue, allocator);
	//d.AddMember(tempname, tempValue, allocator);
	//if (d.HasParseError())
	//{
	//	std::cout << "parse error" << std::endl;
	//}
	////Guards to check for error
	//assert(d.IsObject());
	//assert(d.HasMember("hello"));
	//assert(d["hello"].IsString());
	////Access values using key
	//std::cout << "hello = " << d["hello"].GetString() << std::endl;

	////Using itr to get member values
	//Value::MemberIterator hello = d.FindMember("hello");
	//assert(hello != d.MemberEnd());
	//std::cout << hello->name.GetString() <<" = "<< hello->value.GetString() << std::endl;

	//printf("n = %s\n", d["n"].IsNull() ? "null" : "hi");

	//static const char* kTypeNames[] = { "Null", "False", "True", "Object", "Array", "String", "Number" };
	//for (Value::ConstMemberIterator itr = d.MemberBegin(); itr != d.MemberEnd(); ++itr)
	//	printf("Type of member %s is %s\n", itr->name.GetString(), kTypeNames[itr->value.GetType()]);

	//std::cout << sb.GetString() << std::endl;
	
	FILE* fp = fopen(filename.c_str(), "wb"); // non-Windows use "w"
	char writeBuffer[65536];
	FileWriteStream os(fp, writeBuffer, sizeof(writeBuffer));
	PrettyWriter<FileWriteStream> writer(os);
	gameFile.Accept(writer);
	fclose(fp);
	
}

void GameSave::ResetGame()
{
	FILE* fp = fopen("default.json", "rb"); // non-Windows use "r"

	char readBuffer[65536];
	FileReadStream is(fp, readBuffer, sizeof(readBuffer));

	gameFile.ParseStream(is);
	fclose(fp);
	
	fp = fopen(filename.c_str(), "wb"); // non-Windows use "w"
	char writeBuffer[65536];
	FileWriteStream os(fp, writeBuffer, sizeof(writeBuffer));
	PrettyWriter<FileWriteStream> writer(os);
	gameFile.Accept(writer);
	fclose(fp);
}

rapidjson::Value GameSave::SaveVector3(Vector3 vector)
{
	Document::AllocatorType& allocator = gameFile.GetAllocator();
	Value vectorValue(kObjectType);
	vectorValue.AddMember("x", vector.x, allocator);
	vectorValue.AddMember("y", vector.y, allocator);
	vectorValue.AddMember("z", vector.z, allocator);
	return vectorValue;
}

rapidjson::Value GameSave::SaveVillager(GameObject * go)
{
	Document::AllocatorType& allocator = gameFile.GetAllocator();
	Villager* goVil = static_cast<Villager*>(go);
	Value aVillager(kObjectType);
	aVillager.AddMember("Efficiency", goVil->fEfficiency, allocator);
	aVillager.AddMember("Wood", goVil->iWoodStored, allocator);
	aVillager.AddMember("Stone", goVil->iStoneStored, allocator);
	aVillager.AddMember("Food", goVil->iFoodStored, allocator);
	aVillager.AddMember("State", goVil->eCurrState, allocator);

	Value stats(kArrayType);
	Value aStat(kObjectType);
	aStat.AddMember("Hunting", goVil->fStats[Villager::HUNTING], allocator);
	aStat.AddMember("Foraging", goVil->fStats[Villager::FORAGING], allocator);
	aStat.AddMember("Woodcutting", goVil->fStats[Villager::WOODCUTTING], allocator);
	aStat.AddMember("Building", goVil->fStats[Villager::BUILDING], allocator);
	aStat.AddMember("Breeding", goVil->fStats[Villager::BREEDING], allocator);
	aStat.AddMember("Combat", goVil->fStats[Villager::COMBAT], allocator);
	aStat.AddMember("Mining", goVil->fStats[Villager::MINING], allocator);
	stats.PushBack(aStat, allocator);

	aVillager.AddMember("Stats", stats, allocator);
	aVillager.AddMember("Pos", SaveVector3(goVil->pos), allocator);
	return aVillager;
}

rapidjson::Value GameSave::SaveBuilding(GameObject * go)
{
	Document::AllocatorType& allocator = gameFile.GetAllocator();
	Building* goBuilding = static_cast<Building*>(go);
	Value aBuilding(kObjectType);

	Value aName(kStringType);
	aName.SetString(goBuilding->buildingName.c_str(), goBuilding->buildingName.size());

	aBuilding.AddMember("Name", aName, allocator);
	aBuilding.AddMember("Type", go->type, allocator);
	aBuilding.AddMember("State", goBuilding->eCurrState, allocator);
	aBuilding.AddMember("Tier", goBuilding->eCurrTier, allocator);
	aBuilding.AddMember("Built", goBuilding->bBuilt, allocator);
	aBuilding.AddMember("Build Time", goBuilding->fBuildTime, allocator);
	aBuilding.AddMember("Repair Time", goBuilding->fRepairTime, allocator);
	aBuilding.AddMember("Pos", SaveVector3(goBuilding->pos), allocator);

	Altar* goAltar = dynamic_cast<Altar*>(goBuilding);
	ChiefHut* goChief = dynamic_cast<ChiefHut*>(goBuilding);
	Granary* goGranary = dynamic_cast<Granary*>(goBuilding);
	WoodShed* goWood = dynamic_cast<WoodShed*>(goBuilding);
	Logs* goLogs = dynamic_cast<Logs*>(goBuilding);
	House* goHouse = dynamic_cast<House*>(goBuilding);
	ResearchLab* goLab = dynamic_cast<ResearchLab*>(goBuilding);

	if (goAltar)
	{
		Value altarStuff(kObjectType);
		altarStuff.AddMember("Food Offered", goAltar->iFoodOffered, allocator);
		altarStuff.AddMember("Countdown", goAltar->fCountdown, allocator);
		altarStuff.AddMember("Max Countdown", goAltar->fMaxCountdown, allocator);
		altarStuff.AddMember("God Mad Countdown", goAltar->fGodMad, allocator);
		altarStuff.AddMember("World Ended", goAltar->bWorldEnded, allocator);

		aBuilding.AddMember("Altar Values", altarStuff, allocator);
	}
	else if (goChief)
	{
		Value chiefStuff(kObjectType);
		chiefStuff.AddMember("Housing Space", goChief->iHousingSpace, allocator);

		aBuilding.AddMember("Chief Values", chiefStuff, allocator);
	}
	else if (goGranary)
	{
		Value granaryStuff(kObjectType);
		granaryStuff.AddMember("Food Capacity", goGranary->foodCapacity, allocator);

		aBuilding.AddMember("Granary Values", granaryStuff, allocator);
	}
	else if (goWood)
	{
		Value woodStuff(kObjectType);
		woodStuff.AddMember("Wood Capacity", goWood->woodCapacity, allocator);

		aBuilding.AddMember("Woodshed Values", woodStuff, allocator);
	}
	else if (goLogs)
	{
		Value logStuff(kObjectType);

		aBuilding.AddMember("Log Values", logStuff, allocator);
	}
	else if (goHouse)
	{
		Value houseStuff(kObjectType);
		houseStuff.AddMember("Housing Space", goHouse->iHousingSpace, allocator);

		aBuilding.AddMember("Altar Values", houseStuff, allocator);
	}
	else if (goLab)
	{
		Value labStuff(kObjectType);

		aBuilding.AddMember("Lab Values", labStuff, allocator);
	}

	return aBuilding;
}

rapidjson::Value GameSave::SaveEnvironment(GameObject * go)
{
	Document::AllocatorType& allocator = gameFile.GetAllocator();
	Environment* goEnvironment = static_cast<Environment*>(go);
	Value aEnvironment(kObjectType);

	aEnvironment.AddMember("Type", goEnvironment->type, allocator);
	aEnvironment.AddMember("Timer", goEnvironment->fTimer, allocator);
	aEnvironment.AddMember("Pos", SaveVector3(goEnvironment->pos), allocator);

	Tree* goTree = dynamic_cast<Tree*>(goEnvironment);
	Bush* goBush = dynamic_cast<Bush*>(goEnvironment);
	Mountain* goMountain = dynamic_cast<Mountain*>(goEnvironment);
	
	if (goTree)
	{
		Value treeStuff(kObjectType);
		treeStuff.AddMember("Wood Amount", goTree->iWoodAmount, allocator);
		treeStuff.AddMember("State", goTree->eCurrState, allocator);

		aEnvironment.AddMember("Tree Values", treeStuff, allocator);
	}
	else if (goBush)
	{
		Value bushStuff(kObjectType);
		bushStuff.AddMember("Food Amount", goBush->iFoodAmount, allocator);
		bushStuff.AddMember("State", goBush->eCurrState, allocator);

		aEnvironment.AddMember("Bush Values", bushStuff, allocator);
	}
	else if (goMountain)
	{
		Value mountainStuff(kObjectType);
		mountainStuff.AddMember("Stone Amount", goMountain->iStoneAmount, allocator);
		mountainStuff.AddMember("Stone Gain", goMountain->iStoneGain, allocator);

		aEnvironment.AddMember("Mountain Values", mountainStuff, allocator);
	}
	
	return aEnvironment;
}

rapidjson::Value GameSave::SaveCalamityGo(GameObject * go)
{
	Document::AllocatorType& allocator = gameFile.GetAllocator();

	Value aCalamityGO(kObjectType);



	Tsunami* goTsunami = dynamic_cast<Tsunami*>(go);
	Tornado* goTornado = dynamic_cast<Tornado*>(go);

	Value calamityName(kStringType);
	
	if (goTsunami)
	{
		calamityName.SetString("TsunamiGO", allocator);
		aCalamityGO.AddMember("Name", calamityName, allocator);

		Value tsunamiStuff(kObjectType);
		tsunamiStuff.AddMember("Power", goTsunami->fPower, allocator);
		tsunamiStuff.AddMember("Tsunami Direciton", goTsunami->tsunami_direction, allocator);
		tsunamiStuff.AddMember("Cloud Timer", goTsunami->fParticleTimer_Cloud, allocator);

		aCalamityGO.AddMember("TsunamiGO Values", tsunamiStuff, allocator);
	}
	else if (goTornado)
	{
		calamityName.SetString("TornadoGO", allocator);
		aCalamityGO.AddMember("Name", calamityName, allocator);

		Value tornadoStuff(kObjectType);
		tornadoStuff.AddMember("Power", goTornado->fPower, allocator);
		tornadoStuff.AddMember("Dirt Timer", goTornado->fEffectTimer_Dirt, allocator);
		tornadoStuff.AddMember("Cloud Timer", goTornado->fEffectTimer_Cloud, allocator);
		tornadoStuff.AddMember("Elapsed Time", goTornado->fElapsedTime, allocator);

		aCalamityGO.AddMember("TornadoGO Values", tornadoStuff, allocator);
	}

	aCalamityGO.AddMember("Type", go->type, allocator);
	aCalamityGO.AddMember("Pos", SaveVector3(go->pos), allocator);
	aCalamityGO.AddMember("Speed", go->moveSpeed, allocator);
	return aCalamityGO;
}

rapidjson::Value GameSave::SaveCalamity(CalamityBase * go)
{
	Document::AllocatorType& allocator = gameFile.GetAllocator();

	Value aCalamity(kObjectType);
	Value calamityName(kStringType);



	CalamityBlizzard* cBlizzard = dynamic_cast<CalamityBlizzard*>(go);
	CalamityEarthquake* cEarthquake = dynamic_cast<CalamityEarthquake*>(go);
	CalamityTornado* cTornado = dynamic_cast<CalamityTornado*>(go);
	CalamityTsunami* cTsunami = dynamic_cast<CalamityTsunami*>(go);
	CalamityWorldEnd* cWorldEnd = dynamic_cast<CalamityWorldEnd*>(go);

	if (cBlizzard)
	{
		//Something
		calamityName.SetString("Blizzard", allocator);
		aCalamity.AddMember("Name", calamityName, allocator);

		Value blizzardStuff(kObjectType);
		blizzardStuff.AddMember("UI Timer", cBlizzard->fUIPopupTime, allocator);
		blizzardStuff.AddMember("Blizzard Cloud Timer", cBlizzard->fEffectTimer_BlizzardCloud, allocator);
		blizzardStuff.AddMember("State", cBlizzard->state, allocator);

		aCalamity.AddMember("Earthquake Values", blizzardStuff, allocator);
	}
	else if (cEarthquake)
	{
		calamityName.SetString("Earthquake", allocator);
		aCalamity.AddMember("Name", calamityName, allocator);

		Value earthquakeStuff(kObjectType);
		earthquakeStuff.AddMember("UI Timer", cEarthquake->fUIPopupTime, allocator);
		earthquakeStuff.AddMember("Dirt Timer", cEarthquake->fEffectTimer_Dirt, allocator);
		earthquakeStuff.AddMember("State", cEarthquake->state, allocator);

		aCalamity.AddMember("Earthquake Values", earthquakeStuff, allocator);
	}
	else if (cTornado)
	{
		calamityName.SetString("Tornado", allocator);
		aCalamity.AddMember("Name", calamityName, allocator);

		Value tornadoStuff(kObjectType);
		tornadoStuff.AddMember("UI Timer", cTornado->fUIPopupTime, allocator);
		tornadoStuff.AddMember("Dirt Timer", cTornado->fEffectTimer_Dirt, allocator);
		tornadoStuff.AddMember("State", cTornado->state, allocator);

		aCalamity.AddMember("Tornado Values", tornadoStuff, allocator);
	}
	else if (cTsunami)
	{
		calamityName.SetString("Tsunami", allocator);
		aCalamity.AddMember("Name", calamityName, allocator);

		Value tsunamiStuff(kObjectType);
		tsunamiStuff.AddMember("UI Timer", cTsunami->fUIPopupTime, allocator);
		tsunamiStuff.AddMember("Warn Timer", cTsunami->fWarnTime, allocator);
		tsunamiStuff.AddMember("Duration", cTsunami->fDuration, allocator);
		tsunamiStuff.AddMember("State", cTsunami->state, allocator);
		Value lanes(kObjectType);
		int i = 0;
		Value laneText(kStringType);
		std::string somestring = "";
		for (auto lane : cTsunami->lanes)
		{
			somestring = "Lane" + std::to_string(i);
			laneText.SetString(somestring.c_str(), allocator);
			lanes.AddMember(laneText, lane, allocator);
			++i;
		}
		tsunamiStuff.AddMember("Lanes", lanes, allocator);

		aCalamity.AddMember("Tsunami Values", tsunamiStuff, allocator);
	}
	else if (cWorldEnd)
	{
		calamityName.SetString("World End", allocator);
		aCalamity.AddMember("Name", calamityName, allocator);

		float fUIPopupTime;
		float fEffectTimer_Dirt;
		Value worldEndStuff(kObjectType);
		worldEndStuff.AddMember("UI Timer", cWorldEnd->fUIPopupTime, allocator);
		worldEndStuff.AddMember("Dirt Timer", cWorldEnd->fEffectTimer_Dirt, allocator);

		aCalamity.AddMember("World End Values", worldEndStuff, allocator);
	}
	aCalamity.AddMember("Active", go->bActive, allocator);
	aCalamity.AddMember("Elapsed Time", go->fElapsedTime, allocator);
	aCalamity.AddMember("Calamity Duration", go->fCalamityDuration, allocator);


	return aCalamity;
}

Vector3 GameSave::LoadVector3(rapidjson::Value& vectorValue)
{
	if (!vectorValue.IsObject())
	{
		std::cout << "Vector3 value is not obj" << std::endl;
	}
	//return Vector3(, vectorValue.FindMember("y"), vectorValue.FindMember("z"));
	Vector3 temp;
	temp.x = vectorValue["x"].GetFloat();
	temp.y = vectorValue["y"].GetFloat();
	temp.z = vectorValue["z"].GetFloat();
	return temp;
}

GameObject * GameSave::LoadVillager(rapidjson::Value& villagerValue)
{
	Villager* goVil = new Villager();

	goVil->fEfficiency = villagerValue["Efficiency"].GetFloat();

	return nullptr;
}

GameObject * GameSave::LoadBuilding(rapidjson::Value& buildingValue)
{
	return nullptr;
}

GameObject * GameSave::LoadEnvironment(rapidjson::Value& environmentValue)
{
	return nullptr;
}


GameObject * GameSave::LoadCalamityGo(rapidjson::Value& calamityGOValue)
{
	return nullptr;
}

GameObject * GameSave::LoadCalamity(rapidjson::Value& calamityValue)
{
	return nullptr;
}
