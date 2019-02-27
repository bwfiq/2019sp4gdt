#define _CRT_SECURE_NO_WARNINGS

#include "GameSave.h"
#include "SceneData.h"

#include "Villager.h"
#include "Pig.h"
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

#include "SMManager.h"

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
	SceneData* SD = SceneData::GetInstance();

	struct stat statbuffer;
	FILE* fp;

	if (stat(filename.c_str(), &statbuffer) != 0)
	{
		std::cout << "hi, file does not exist, rapidjson" << std::endl;
		fp = fopen("default.json", "rb"); // non-Windows use "r"
	}
	else
	{
		fp = fopen(filename.c_str(), "rb"); // non-Windows use "r"
	}
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
		//StringBuffer buffer;
		//PrettyWriter<StringBuffer> writer(buffer);
		//gameFile.Accept(writer);
		//std::cout << buffer.GetString() << std::endl;

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
		
		if (gameFile.IsObject())
		{
			if (gameFile.HasMember("Villagers"))
			{
				objVillagers = gameFile["Villagers"];
			}
			else
			{
				std::cout << "Cannot find Villagers Data" << std::endl;
				return false;
			}
			if (gameFile.HasMember("Buildings"))
			{
				objBuildings = gameFile["Buildings"];
			}
			else
			{
				std::cout << "Cannot find Buildings Data" << std::endl;
				return false;
			}
			if (gameFile.HasMember("Environments"))
			{
				objEnvironments = gameFile["Environments"];
			}
			else
			{
				std::cout << "Cannot find Environments Data" << std::endl;
				return false;
			}
			if (gameFile.HasMember("CalamityGO"))
			{
				objCalamityGO = gameFile["CalamityGO"];
			}
			else
			{
				std::cout << "Cannot find CalamityGO Data" << std::endl;
				return false;
			}
			if (gameFile.HasMember("Calamities"))
			{
				objCalamities = gameFile["Calamities"];
			}
			else
			{
				std::cout << "Cannot find Calamities Data" << std::endl;
				return false;
			}
		}
		else
		{
			std::cout << "JSON file is empty" << std::endl;
			return false;
		}

		std::vector<GameObject*>* m_goList = SD->GetGOList();
		while (!m_goList->empty())
		{
			GameObject* temp = m_goList->back();
			m_goList->pop_back();
			delete temp;
		}
		
		//Guards to check for json file error
		if (objVillagers.HasMember("Number"))
		{
			numVil = objVillagers["Number"].GetInt();
		}
		else
		{
			std::cout << "Cannot find Number in Villagers OBJ" << std::endl;
			return false;
		}

		if (objBuildings.HasMember("Number"))
		{
			numBuilding = objBuildings["Number"].GetInt();
		}
		else
		{
			std::cout << "Cannot find Number in Buildings OBJ" << std::endl;
			return false;
		}
		
		if (objEnvironments.HasMember("Number"))
		{
			numEnvironment = objEnvironments["Number"].GetInt();
		}
		else
		{
			std::cout << "Cannot find Number in Environments OBJ" << std::endl;
			return false;
		}

		if (objCalamityGO.HasMember("Number"))
		{
			numCalamitiesGO = objCalamityGO["Number"].GetInt();
		}
		else
		{
			std::cout << "Cannot find Number in CalamityGO OBJ" << std::endl;
			return false;
		}

		if (objCalamities.HasMember("Number"))
		{
			numCalamities = objCalamities["Number"].GetInt();
		}
		else
		{
			std::cout << "Cannot find Number in Calamities OBJ" << std::endl;
			return false;
		}

		if (objCalamities.HasMember("Number Queue"))
		{
			numCalamitiesQ = objCalamities["Number Queue"].GetInt();
		}
		else
		{
			std::cout << "Cannot find Number Queue in Calamities OBJ" << std::endl;
			return false;
		}

		//Loading all the stuff

		//Load Villagers
		if (objVillagers.HasMember("Villager"))
		{
			for (int i = 0; i < numVil; ++i)
			{
				m_goList->push_back(LoadVillager(objVillagers["Villager"][i]));
			}
		}
		else
		{
			std::cout << "Cannot find Villager in Villagers OBJ" << std::endl;
			return false;
		}

		//Load Buildings
		if (objBuildings.HasMember("Building"))
		{
			for (int i = 0; i < numBuilding; ++i)
			{
				m_goList->push_back(LoadBuilding(objBuildings["Building"][i]));
			}
		}
		else
		{
			std::cout << "Cannot find Building in Buildings OBJ" << std::endl;
			return false;
		}
		
		//Load Environments
		if (objEnvironments.HasMember("Environment"))
		{
			for (int i = 0; i < numEnvironment; ++i)
			{
				m_goList->push_back(LoadEnvironment(objEnvironments["Environment"][i]));
			}
		}
		else
		{
			std::cout << "Cannot find Environment in Environments OBJ" << std::endl;
			return false;
		}
		
		//Load CalamitiesGO
		if (objCalamityGO.HasMember("Calamities GO"))
		{
			for (int i = 0; i < numCalamitiesGO; ++i)
			{
				//m_goList->push_back(LoadCalamityGo(objCalamityGO["Calamities GO"][i]));
			}
		}
		else
		{
			std::cout << "Cannot find Calamities GO in CalamityGO OBJ" << std::endl;
			return false;
		}

		//Load Calamities
		if (objCalamities.HasMember("Calamities"))
		{
			for (int i = 0; i < numCalamities; ++i)
			{
				//LoadCalamity(objCalamities["Calamities"][i]);
			}
		}
		else
		{
			std::cout << "Cannot find Calamities in Calamities OBJ" << std::endl;
			return false;
		}
		
		//Load CalamitiesQ
		if (objCalamities.HasMember("CalamitiesQ"))
		{
			for (int i = 0; i < numCalamitiesQ; ++i)
			{
				//LoadCalamity(objCalamities["CalamitiesQ"][i]);
			}
		}
		else
		{
			std::cout << "Cannot find CalamitiesQ in Calamities OBJ" << std::endl;
			return false;
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
	Value objPigs(kObjectType);
	Value objBuildings(kObjectType);
	Value objEnvironments(kObjectType);
	Value objCalamityGO(kObjectType);
	Value objCalamities(kObjectType);

	int numVil = 0;
	int numPig = 0;
	int numBuilding = 0;
	int numEnvironment = 0;
	int numCalamitiesGO = 0;
	int numCalamities = 0;
	int numCalamitiesQ = 0;
	Value villagers(kArrayType);
	Value pigs(kArrayType);
	Value buildings(kArrayType);
	Value environments(kArrayType);
	Value calamitiesGO(kArrayType);
	Value calamities(kArrayType);
	Value calamitiesQ(kArrayType);
	
	//Save all GameObjects
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
		Pig* goPig = dynamic_cast<Pig*>(go);
		if (goPig)
		{
			pigs.PushBack(SavePig(go), allocator);
			++numPig;

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

	//Save current Calamity
	if (CM->GetCurrCalamity() != NULL)
	{
		calamities.PushBack(SaveCalamity(CM->GetCurrCalamity()), allocator);
		++numCalamities;
	}

	//Save Calamity Queue
	std::queue<CalamityBase*> calamityQueue = CM->GetCalamityQueue();
	while (!calamityQueue.empty())
	{
		++numCalamitiesQ;
		calamitiesQ.PushBack(SaveCalamity(calamityQueue.front()), allocator);
		calamityQueue.pop();
	}

	objVillagers.AddMember("Number", numVil, allocator);
	objVillagers.AddMember("Villager", villagers, allocator);

	objPigs.AddMember("Number", numPig, allocator);
	objPigs.AddMember("Pig", pigs, allocator);

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
	gameFile.AddMember("Pigs", objPigs, allocator);
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

rapidjson::Value GameSave::SavePig(GameObject * go)
{
	Document::AllocatorType& allocator = gameFile.GetAllocator();
	Pig* goPig = static_cast<Pig*>(go);
	Value aPig(kObjectType);
	aPig.AddMember("State", goPig->state, allocator);
	aPig.AddMember("Movement", goPig->movement, allocator);
	aPig.AddMember("Food", goPig->iFoodAmount, allocator);
	aPig.AddMember("Timer", goPig->fTimer, allocator);
	aPig.AddMember("Energy", goPig->fEnergy, allocator);
	aPig.AddMember("Idle Timer", goPig->fIdleTimer, allocator);
	aPig.AddMember("Action Timer", goPig->fActionTimer, allocator);


	aPig.AddMember("Pos", SaveVector3(goPig->pos), allocator);
	return aPig;
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

		aBuilding.AddMember("House Values", houseStuff, allocator);
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
	//return Vector3(, vectorValue.FindMember("y"), vectorValue.FindMember("z"));
	Vector3 temp;

	//Loading x
	if (vectorValue.HasMember("x"), vectorValue["x"].IsFloat())
	{
		temp.x = vectorValue["x"].GetFloat();
	}
	else
	{
		std::cout << "Error Loading Vector3 x" << std::endl;
		return NULL;
	}

	//Loading y
	if (vectorValue.HasMember("y"), vectorValue["y"].IsFloat())
	{
		temp.y = vectorValue["y"].GetFloat();
	}
	else
	{
		std::cout << "Error Loading Vector3 y" << std::endl;
		return NULL;
	}

	//Loading z
	if (vectorValue.HasMember("z"), vectorValue["z"].IsFloat())
	{
		temp.z = vectorValue["z"].GetFloat();
	}
	else
	{
		std::cout << "Error Loading Vector3 z" << std::endl;
		return NULL;
	}
	return temp;
}

GameObject* GameSave::LoadVillager(rapidjson::Value& villagerValue)
{
	Villager* goVil = NULL;
	if (villagerValue.IsObject())
	{
		goVil = new Villager(GameObject::GO_VILLAGER);
	}
	else
	{
		std::cout << "Error with villagerValue type" << std::endl;
		return nullptr;
	}

	//Loading Efficiency
	if (villagerValue.HasMember("Efficiency") && villagerValue["Efficiency"].IsFloat())
	{
		goVil->fEfficiency = villagerValue["Efficiency"].GetFloat();
	}
	else
	{
		std::cout << "Error Loading villager Efficiency" << std::endl;
		if (goVil != NULL)
		{
			delete goVil;
			goVil = NULL;
		}
		return nullptr;
	}

	//Loading Wood
	if (villagerValue.HasMember("Wood") && villagerValue["Wood"].IsInt())
	{
		goVil->iWoodStored = villagerValue["Wood"].GetInt();
	}
	else
	{
		std::cout << "Error Loading villager Wood" << std::endl;
		if (goVil != NULL)
		{
			delete goVil;
			goVil = NULL;
		}
		return nullptr;
	}

	//Loading Stone
	if (villagerValue.HasMember("Stone") && villagerValue["Stone"].IsInt())
	{
		goVil->iStoneStored = villagerValue["Stone"].GetInt();
	}
	else
	{
		std::cout << "Error Loading villager Stone" << std::endl;
		if (goVil != NULL)
		{
			delete goVil;
			goVil = NULL;
		}
		return nullptr;
	}

	//Loading Food
	if (villagerValue.HasMember("Food") && villagerValue["Food"].IsInt())
	{
		goVil->iFoodStored = villagerValue["Food"].GetInt();
	}
	else
	{
		std::cout << "Error Loading villager Food" << std::endl;
		if (goVil != NULL)
		{
			delete goVil;
			goVil = NULL;
		}
		return nullptr;
	}

	//Loading State
	if (villagerValue.HasMember("State") && villagerValue["State"].IsInt())
	{
		goVil->eCurrState = (Villager::STATES)(villagerValue["State"].GetInt());
	}
	else
	{
		std::cout << "Error Loading villager State" << std::endl;
		if (goVil != NULL)
		{
			delete goVil;
			goVil = NULL;
		}
		return nullptr;
	}

	//Loading State
	if (villagerValue.HasMember("Pos") && villagerValue["Pos"].IsObject())
	{
		goVil->pos = LoadVector3(villagerValue["Pos"]);
		if (goVil->pos == NULL)
		{
			std::cout << "Error Loading Vector3 Villager Pos" << std::endl;
			delete goVil;
			goVil = NULL;
			return nullptr;
		}
	}
	else
	{
		std::cout << "Error Loading villager Pos" << std::endl;
		if (goVil != NULL)
		{
			delete goVil;
			goVil = NULL;
		}
		return nullptr;
	}

	if (villagerValue.HasMember("Stats") && villagerValue["Stats"].IsArray())
	{
		if (villagerValue["Stats"][0].IsObject())
		{
			goVil->fStats[Villager::HUNTING] = villagerValue["Stats"][0]["Hunting"].GetFloat();
			goVil->fStats[Villager::FORAGING] = villagerValue["Stats"][0]["Foraging"].GetFloat();
			goVil->fStats[Villager::WOODCUTTING] = villagerValue["Stats"][0]["Woodcutting"].GetFloat();
			goVil->fStats[Villager::BUILDING] = villagerValue["Stats"][0]["Building"].GetFloat();
			goVil->fStats[Villager::BREEDING] = villagerValue["Stats"][0]["Breeding"].GetFloat();
			goVil->fStats[Villager::COMBAT] = villagerValue["Stats"][0]["Combat"].GetFloat();
			goVil->fStats[Villager::MINING] = villagerValue["Stats"][0]["Mining"].GetFloat();
		}
	}
	else
	{
		std::cout << "Error Loading villager Stats" << std::endl;
		if (goVil != NULL)
		{
			delete goVil;
			goVil = NULL;
		}
		return nullptr;
	}

	goVil->smID = "VillagerSM";
	goVil->m_currState = goVil->m_nextState = SMManager::GetInstance()->GetSM(goVil->smID)->GetState("Idle");
	goVil->active = true;

	return goVil;
}

GameObject* GameSave::LoadBuilding(rapidjson::Value& buildingValue)
{
	std::string name;
	if (buildingValue["Name"].IsString())
	{
		name = buildingValue["Name"].GetString();
	}
	else
	{
		std::cout << "Error Loading building name" << std::endl;
		return nullptr;
	}
	GameObject::GAMEOBJECT_TYPE type;
	if (buildingValue.HasMember("Type") && buildingValue["Type"].IsInt())
	{
		type = (GameObject::GAMEOBJECT_TYPE)(buildingValue["Type"].GetInt());
		if ((int)(type) >= (int)(GameObject::GO_TOTAL))
		{
			std::cout << "Error Building Type" << std::endl;
			return nullptr;
		}
	}
	else
	{
		std::cout << "Error Loading Building, Unkown type";
		return nullptr;
	}
	Building* go = NULL;
	if (name == "Chief Hut")
	{
		ChiefHut* goChiefhut = new ChiefHut(GameObject::GO_CHIEFHUT);
		
		//Loading Chief Values
		if (buildingValue.HasMember("Chief Values") && buildingValue["Chief Values"].IsObject())
		{
			//Loading Housing Space
			if (buildingValue["Chief Values"].HasMember("Housing Space") && buildingValue["Chief Values"]["Housing Space"].IsInt())
			{
				goChiefhut->iHousingSpace = buildingValue["Chief Values"]["Housing Space"].GetInt();
			}
			else
			{
				std::cout << "Error Loading Chief values housing space" << std::endl;
				if (goChiefhut != NULL)
				{
					delete goChiefhut;
					goChiefhut = NULL;
				}
				return nullptr;
			}
		}
		else
		{
			std::cout << "Error Loading Chief values" << std::endl;
			if (goChiefhut != NULL)
			{
				delete goChiefhut;
				goChiefhut = NULL;
			}
			return nullptr;
		}
		go = goChiefhut;
	}
	else if (name == "House")
	{
		House* goHouse = new House(GameObject::GO_HOUSE);

		//Loading House Values
		if (buildingValue.HasMember("House Values") && buildingValue["House Values"].IsObject())
		{
			//Loading Housing Space
			if (buildingValue["House Values"].HasMember("Housing Space") && buildingValue["House Values"]["Housing Space"].IsInt())
			{
				goHouse->iHousingSpace = buildingValue["House Values"]["Housing Space"].GetInt();
			}
			else
			{
				std::cout << "Error Loading House Values housing space" << std::endl;
				if (goHouse != NULL)
				{
					delete goHouse;
					goHouse = NULL;
				}
				return nullptr;
			}
		}
		else
		{
			std::cout << "Error Loading House values" << std::endl;
			if (goHouse != NULL)
			{
				delete goHouse;
				goHouse = NULL;
			}
			return nullptr;
		}
		go = goHouse;
	}
	else if (name == "Granary")
	{
		Granary* goGranary = new Granary(GameObject::GO_GRANARY);

		//Loading Granary Values
		if (buildingValue.HasMember("Granary Values") && buildingValue["Granary Values"].IsObject())
		{
			//Loading Food Capacity
			if (buildingValue["Granary Values"].HasMember("Food Capacity") && buildingValue["Granary Values"]["Food Capacity"].IsInt())
			{
				goGranary->foodCapacity = buildingValue["Granary Values"]["Food Capacity"].GetInt();
			}
			else
			{
				std::cout << "Error Loading Granary Values Food Capacity" << std::endl;
				if (goGranary != NULL)
				{
					delete goGranary;
					goGranary = NULL;
				}
				return nullptr;
			}
		}
		else
		{
			std::cout << "Error Loading Granary Values" << std::endl;
			if (goGranary != NULL)
			{
				delete goGranary;
				goGranary = NULL;
			}
			return nullptr;
		}
		go = goGranary;
	}
	else if (name == "WoodShed")
	{
		WoodShed* goWoodShed = new WoodShed(GameObject::GO_WOODSHED);

		//Loading Woodshed Values
		if (buildingValue.HasMember("Woodshed Values") && buildingValue["Woodshed Values"].IsObject())
		{
			//Loading Wood Capacity
			if (buildingValue["Woodshed Values"].HasMember("Wood Capacity") && buildingValue["Woodshed Values"]["Wood Capacity"].IsInt())
			{
				goWoodShed->woodCapacity = buildingValue["Woodshed Values"]["Wood Capacity"].GetInt();
			}
			else
			{
				std::cout << "Error Loading Woodshed Values Wood Capacity" << std::endl;
				if (goWoodShed != NULL)
				{
					delete goWoodShed;
					goWoodShed = NULL;
				}
				return nullptr;
			}
		}
		else
		{
			std::cout << "Error Loading Woodshed Values" << std::endl;
			if (goWoodShed != NULL)
			{
				delete goWoodShed;
				goWoodShed = NULL;
			}
			return nullptr;
		}
		go = goWoodShed;
	}
	else if (name == "Logs")
	{
		Logs* goLogs = new Logs(GameObject::GO_LOGS);
		
		//Loading Log Values
		if (buildingValue.HasMember("Log Values") && buildingValue["Log Values"].IsObject())
		{
		}
		else
		{
			std::cout << "Error Loading Log Values" << std::endl;
			if (goLogs != NULL)
			{
				delete goLogs;
				goLogs = NULL;
			}
			return nullptr;
		}
		go = goLogs;
	}
	else if (name == "Research Lab")
	{
		ResearchLab* goLab = new ResearchLab(GameObject::GO_RESEARCHLAB);

		//Loading Lab Values
		if (buildingValue.HasMember("Lab Values") && buildingValue["Lab Values"].IsObject())
		{
		}
		else
		{
			std::cout << "Error Loading Lab Values" << std::endl;
			if (goLab != NULL)
			{
				delete goLab;
				goLab = NULL;
			}
			return nullptr;
		}
		go = goLab;
	}
	else if (name == "Altar")
	{
		Altar* goAltar = new Altar(GameObject::GO_ALTAR);

		//Loading Altar Values
		if (buildingValue.HasMember("Altar Values") && buildingValue["Altar Values"].IsObject())
		{
			//Loading Food Offered
			if (buildingValue["Altar Values"].HasMember("Food Offered") && buildingValue["Altar Values"]["Food Offered"].IsInt())
			{
				goAltar->iFoodOffered = buildingValue["Altar Values"]["Food Offered"].GetInt();
			}
			else
			{
				std::cout << "Error Loading Altar Values Food Offered" << std::endl;
				if (goAltar != NULL)
				{
					delete goAltar;
					goAltar = NULL;
				}
				return nullptr;
			}

			//Loading Countdown
			if (buildingValue["Altar Values"].HasMember("Countdown") && buildingValue["Altar Values"]["Countdown"].IsFloat())
			{
				goAltar->fCountdown = buildingValue["Altar Values"]["Countdown"].GetFloat();
			}
			else
			{
				std::cout << "Error Loading Altar Values Countdown" << std::endl;
				if (goAltar != NULL)
				{
					delete goAltar;
					goAltar = NULL;
				}
				return nullptr;
			}

			//Loading Max Countdown
			if (buildingValue["Altar Values"].HasMember("Max Countdown") && buildingValue["Altar Values"]["Max Countdown"].IsFloat())
			{
				goAltar->fMaxCountdown = buildingValue["Altar Values"]["Max Countdown"].GetFloat();
			}
			else
			{
				std::cout << "Error Loading Altar Values Max Countdown" << std::endl;
				if (goAltar != NULL)
				{
					delete goAltar;
					goAltar = NULL;
				}
				return nullptr;
			}

			//Loading God Mad Countdown
			if (buildingValue["Altar Values"].HasMember("God Mad Countdown") && buildingValue["Altar Values"]["God Mad Countdown"].IsFloat())
			{
				goAltar->fGodMad = buildingValue["Altar Values"]["God Mad Countdown"].GetFloat();
			}
			else
			{
				std::cout << "Error Loading Altar Values God Mad Countdown" << std::endl;
				if (goAltar != NULL)
				{
					delete goAltar;
					goAltar = NULL;
				}
				return nullptr;
			}

			//Loading World Ended
			if (buildingValue["Altar Values"].HasMember("World Ended") && buildingValue["Altar Values"]["World Ended"].IsBool())
			{
				goAltar->bWorldEnded = buildingValue["Altar Values"]["World Ended"].GetBool();
			}
			else
			{
				std::cout << "Error Loading Altar Values World Ended" << std::endl;
				if (goAltar != NULL)
				{
					delete goAltar;
					goAltar = NULL;
				}
				return nullptr;
			}
		}
		else
		{
			std::cout << "Error Loading Altar Values" << std::endl;
			if (goAltar != NULL)
			{
				delete goAltar;
				goAltar = NULL;
			}
			return nullptr;
		}
		go = goAltar;
	}
	else
	{
		std::cout << "Error Loading building, Unknown building name";
		if (go != NULL)
		{
			delete go;
			go = NULL;
		}
		return nullptr;
	}
	//"Type": 9,
	//	"State" : 0,
	//	"Tier" : 0,
	//	"Built" : true,
	//	"Build Time" : 4.0,
	//	"Repair Time" : 4.0,
	//	"Pos" : {
	//	"x": -1.0,
	//		"y" : 0.5,
	//		"z" : -5.0
	//},

	//Loading State
	if (buildingValue.HasMember("State") && buildingValue["State"].IsInt() && buildingValue["State"].GetInt() < (int)(Building::STATES_TOTAL))
	{
		go->eCurrState = (Building::STATES)(buildingValue["State"].GetInt());
	}
	else
	{
		std::cout << "Error Loading State" << std::endl;
		if (go != NULL)
		{
			delete go;
			go = NULL;
		}
		return nullptr;
	}

	//Loading Tier
	if (buildingValue.HasMember("Tier") && buildingValue["Tier"].IsInt() && buildingValue["Tier"].GetInt() < (int)(Building::TIER_TOTAL))
	{
		go->eCurrTier = (Building::TIER)(buildingValue["Tier"].GetInt());
	}
	else
	{
		std::cout << "Error Loading Tier" << std::endl;
		if (go != NULL)
		{
			delete go;
			go = NULL;
		}
		return nullptr;
	}

	//Loading Built
	if (buildingValue.HasMember("Built") && buildingValue["Built"].IsBool())
	{
		go->bBuilt = buildingValue["Built"].GetBool();
	}
	else
	{
		std::cout << "Error Loading Built" << std::endl;
		if (go != NULL)
		{
			delete go;
			go = NULL;
		}
		return nullptr;
	}

	//Loading Build Time
	if (buildingValue.HasMember("Build Time") && buildingValue["Build Time"].IsFloat())
	{
		go->fBuildTime = buildingValue["Build Time"].GetFloat();
	}
	else
	{
		std::cout << "Error Loading Build Time" << std::endl;
		if (go != NULL)
		{
			delete go;
			go = NULL;
		}
		return nullptr;
	}

	//Loading Repair Time
	if (buildingValue.HasMember("Repair Time") && buildingValue["Repair Time"].IsFloat())
	{
		go->fRepairTime = buildingValue["Repair Time"].GetFloat();
	}
	else
	{
		std::cout << "Error Loading Repair Time" << std::endl;
		if (go != NULL)
		{
			delete go;
			go = NULL;
		}
		return nullptr;
	}

	//Loading Pos
	if (buildingValue.HasMember("Pos") && buildingValue["Pos"].IsObject())
	{
		go->pos = LoadVector3(buildingValue["Pos"]);
		if (go->pos == NULL)
		{
			std::cout << "Error Loading Vector3 Building Pos" << std::endl;

			delete go;
			go = NULL;
			return nullptr;
		}
	}
	else
	{
		std::cout << "Error Loading Repair Time" << std::endl;
		if (go != NULL)
		{
			delete go;
			go = NULL;
		}
		return nullptr;
	}

	if (go == NULL)
	{
		std::cout << "Error, building is NULL";
		return nullptr;
	}
	go->active = true;
	return go;
}

GameObject* GameSave::LoadEnvironment(rapidjson::Value& environmentValue)
{
	Environment* go = NULL;

	if (!environmentValue.IsObject())
	{
		std::cout << "environmentValue is not a Object" << std::endl;
		return NULL;
	}

	//Loading Type
	GameObject::GAMEOBJECT_TYPE type;
	if (environmentValue.HasMember("Type") && environmentValue["Type"].IsInt() && environmentValue["Type"].GetInt() < (int)(GameObject::GO_TOTAL))
	{
		type = (GameObject::GAMEOBJECT_TYPE)(environmentValue["Type"].GetInt());
	}
	else
	{
		std::cout << "Error Loading Environment Type" << std::endl;
		return nullptr;
	}

	//new GameObject based on type
	switch (type)
	{
	case GameObject::GO_BUSH:
	{	
		Bush* goBush = new Bush(type);

		//Loading Bush Values
		if (environmentValue.HasMember("Bush Values") && environmentValue["Bush Values"].IsObject())
		{
			//Loading Food Amount
			if (environmentValue["Bush Values"].HasMember("Food Amount") && environmentValue["Bush Values"]["Food Amount"].IsInt())
			{
				goBush->iFoodAmount = environmentValue["Bush Values"]["Food Amount"].GetInt();
			}
			else
			{
				std::cout << "Error Loading Bush Values Food Amount" << std::endl;
				if (goBush != NULL)
				{
					delete goBush;
					goBush = NULL;
				}
				return nullptr;
			}

			//Loading State
			if (environmentValue["Bush Values"].HasMember("State") && environmentValue["Bush Values"]["State"].IsInt() && environmentValue["Bush Values"]["State"].GetInt() < (int)(Bush::STATES_TOTAL))
			{
				goBush->eCurrState = (Bush::STATES)(environmentValue["Bush Values"]["State"].GetInt());
			}
			else
			{
				std::cout << "Error Loading Bush Values State" << std::endl;
				if (goBush != NULL)
				{
					delete goBush;
					goBush = NULL;
				}
				return nullptr;
			}
		}
		else
		{
			std::cout << "Error Loading Bush Values" << std::endl;
			if (goBush != NULL)
			{
				delete goBush;
				goBush = NULL;
			}
			return nullptr;
		}
		go = goBush; 
	}
		break;
	case GameObject::GO_MOUNTAIN:
	{	
		Mountain* goMountain = new Mountain(type);

		//Loading Mountain Values
		if (environmentValue.HasMember("Mountain Values") && environmentValue["Mountain Values"].IsObject())
		{
			//Loading Stone Amount
			if (environmentValue["Mountain Values"].HasMember("Stone Amount") && environmentValue["Mountain Values"]["Stone Amount"].IsInt())
			{
				goMountain->iStoneAmount = environmentValue["Mountain Values"]["Stone Amount"].GetInt();
			}
			else
			{
				std::cout << "Error Loading Mountain Values Stone Amount" << std::endl;
				if (goMountain != NULL)
				{
					delete goMountain;
					goMountain = NULL;
				}
				return nullptr;
			}

			//Loading Stone Gain
			if (environmentValue["Mountain Values"].HasMember("Stone Gain") && environmentValue["Mountain Values"]["Stone Gain"].IsInt())
			{
				goMountain->iStoneGain = environmentValue["Mountain Values"]["Stone Gain"].GetInt();
			}
			else
			{
				std::cout << "Error Loading Mountain Values Stone Gain" << std::endl;
				if (goMountain != NULL)
				{
					delete goMountain;
					goMountain = NULL;
				}
				return nullptr;
			}
		}
		else
		{
			std::cout << "Error Loading Mountain Values" << std::endl;
			if (goMountain != NULL)
			{
				delete goMountain;
				goMountain = NULL;
			}
			return nullptr;
		}
		go = goMountain; 
	}
		break;
	case GameObject::GO_TREE:
	{
		Tree* goTree = new Tree(type);

		//Loading Tree Values
		if (environmentValue.HasMember("Tree Values") && environmentValue["Tree Values"].IsObject())
		{
			//Loading Wood Amount
			if (environmentValue["Tree Values"].HasMember("Wood Amount") && environmentValue["Tree Values"]["Wood Amount"].IsInt())
			{
				goTree->iWoodAmount = environmentValue["Tree Values"]["Wood Amount"].GetInt();
			}
			else
			{
				std::cout << "Error Loading Tree Values Wood Amount" << std::endl;
				if (goTree != NULL)
				{
					delete goTree;
					goTree = NULL;
				}
				return nullptr;
			}

			//Loading State
			if (environmentValue["Tree Values"].HasMember("State") && environmentValue["Tree Values"]["State"].IsInt() && environmentValue["Tree Values"]["State"].GetInt() < (int)(Tree::STATES_TOTAL))
			{
				goTree->eCurrState = (Tree::STATES)(environmentValue["Tree Values"]["State"].GetInt());
			}
			else
			{
				std::cout << "Error Loading Tree Values State" << std::endl;
				if (goTree != NULL)
				{
					delete goTree;
					goTree = NULL;
				}
				return nullptr;
			}
		}
		else
		{
			std::cout << "Error Loading Tree Values" << std::endl;
			if (goTree != NULL)
			{
				delete goTree;
				goTree = NULL;
			}
			return nullptr;
		}
		go = goTree;
	}
		break;
	default:
		std::cout << "Error Environment Type" << std::endl;
		return nullptr;
	}

	if (go == NULL)
	{
		std::cout << "Error Loading GameObject" << std::endl;
		return nullptr;
	}

	if (environmentValue.HasMember("Timer") && environmentValue["Timer"].IsFloat())
	{
		go->fTimer = environmentValue["Timer"].GetFloat();
	}
	else
	{
		std::cout << "Error Loading Environment Timer"<<std::endl;
		if (go != NULL)
		{
			delete go;
			go = NULL;
		}
		return nullptr;
	}

	if (environmentValue.HasMember("Pos") && environmentValue["Pos"].IsObject())
	{
		go->pos = LoadVector3(environmentValue["Pos"]);
	}
	else
	{
		std::cout << "Error Loading Environment Pos" << std::endl;
		if (go != NULL)
		{
			delete go;
			go = NULL;
		}
		return nullptr;
	}
	go->active = true;
	return go;
}


GameObject* GameSave::LoadCalamityGo(rapidjson::Value& calamityGOValue)
{
	return nullptr;
}

GameObject * GameSave::LoadPig(rapidjson::Value & pigValue)
{
	Pig* goPig = NULL;
	if (pigValue.IsObject())
	{
		goPig = new Pig(GameObject::GO_PIG);
	}
	else
	{
		std::cout << "Error with pigValue type" << std::endl;
		return nullptr;
	}

	//Loading Food
	if (pigValue.HasMember("Food") && pigValue["Food"].IsInt())
	{
		goPig->iFoodAmount = pigValue["Food"].GetInt();
	}
	else
	{
		std::cout << "Error Loading pig Food" << std::endl;
		if (goPig != NULL)
		{
			delete goPig;
			goPig = NULL;
		}
		return nullptr;
	}

	//Loading Food
	if (pigValue.HasMember("Timer") && pigValue["Timer"].IsFloat())
	{
		goPig->fTimer = pigValue["Timer"].GetFloat();
	}
	else
	{
		std::cout << "Error Loading pig Timer" << std::endl;
		if (goPig != NULL)
		{
			delete goPig;
			goPig = NULL;
		}
		return nullptr;
	}

	//Loading Energy
	if (pigValue.HasMember("Energy") && pigValue["Energy"].IsFloat())
	{
		goPig->fEnergy = pigValue["Energy"].GetFloat();
	}
	else
	{
		std::cout << "Error Loading pig Energy" << std::endl;
		if (goPig != NULL)
		{
			delete goPig;
			goPig = NULL;
		}
		return nullptr;
	}

	//Loading Idle Timer
	if (pigValue.HasMember("Idle Timer") && pigValue["Idle Timer"].IsFloat())
	{
		goPig->fIdleTimer = pigValue["Idle Timer"].GetFloat();
	}
	else
	{
		std::cout << "Error Loading pig Idle Timer" << std::endl;
		if (goPig != NULL)
		{
			delete goPig;
			goPig = NULL;
		}
		return nullptr;
	}

	//Loading Action Timer
	if (pigValue.HasMember("Action Timer") && pigValue["Action Timer"].IsFloat())
	{
		goPig->fEnergy = pigValue["Action Timer"].GetFloat();
	}
	else
	{
		std::cout << "Error Loading pig Action Timer" << std::endl;
		if (goPig != NULL)
		{
			delete goPig;
			goPig = NULL;
		}
		return nullptr;
	}

	//Loading State
	if (pigValue.HasMember("State") && pigValue["State"].IsInt())
	{
		goPig->state = (Pig::State)(pigValue["State"].GetInt());
	}
	else
	{
		std::cout << "Error Loading pig State" << std::endl;
		if (goPig != NULL)
		{
			delete goPig;
			goPig = NULL;
		}
		return nullptr;
	}

	//Loading Movement
	if (pigValue.HasMember("Movement") && pigValue["Movement"].IsInt())
	{
		goPig->movement = (Pig::Movement)(pigValue["Movement"].GetInt());
	}
	else
	{
		std::cout << "Error Loading pig Movement" << std::endl;
		if (goPig != NULL)
		{
			delete goPig;
			goPig = NULL;
		}
		return nullptr;
	}

	//Loading Pos
	if (pigValue.HasMember("Pos") && pigValue["Pos"].IsObject())
	{
		goPig->pos = LoadVector3(pigValue["Pos"]);
		if (goPig->pos == NULL)
		{
			std::cout << "Error Loading Vector3 Pig Pos" << std::endl;
			delete goPig;
			goPig = NULL;
			return nullptr;
		}
	}
	else
	{
		std::cout << "Error Loading villager Pos" << std::endl;
		if (goPig != NULL)
		{
			delete goPig;
			goPig = NULL;
		}
		return nullptr;
	}


	goPig->smID = "PigSM";
	goPig->m_currState = goPig->m_nextState = SMManager::GetInstance()->GetSM(goPig->smID)->GetState("Idle");
	goPig->active = true;

	return goPig;
}

CalamityBase* GameSave::LoadCalamity(rapidjson::Value& calamityValue)
{
	return nullptr;
}
