#define _CRT_SECURE_NO_WARNINGS

#include "GameSave.h"
#include "Villager.h"


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

using namespace rapidjson;
GameSave::GameSave()
{
}

GameSave::~GameSave()
{
}

void GameSave::LoadGame()
{
	struct stat statbuffer;

	if (stat("output.json", &statbuffer) != 0)
	{
		std::cout << "hi, file does not exist, rapidjson" << std::endl;
	}
	else

	{
		FILE* fp = fopen("output.json", "rb"); // non-Windows use "r"

		char readBuffer[65536];
		FileReadStream is(fp, readBuffer, sizeof(readBuffer));
		Document d;
		d.ParseStream(is);
		if (d.HasParseError())
		{
			std::cout << "parse error" << std::endl;
		}
		fclose(fp);
		StringBuffer buffer;
		PrettyWriter<StringBuffer> writer(buffer);
		d.Accept(writer);
		std::cout << buffer.GetString() << std::endl;
	}
}
void GameSave::SaveGame()
{
	Villager temp;
	const char json[] = " { \"hello\" : \"world\", \"t\" : true , \"f\" : false, \"n\": null, \"i\":123, \"pi\": 3.1416, \"a\":[1, 2, 3, 4] } ";
	
	Document d;
	//d.Parse(json);
	d.SetObject();
	Document::AllocatorType& allocator = d.GetAllocator();

	//d.AddMember("version", 1, allocator);
	//d.AddMember("testId", 2, allocator);
	//d.AddMember("group", 3, allocator);
	//d.AddMember("order", 4, allocator);

	Value obj(kObjectType);
	obj.AddMember("Number", 1, allocator);

	Value villagers(kArrayType);
	
		Value aVillager(kObjectType);
		aVillager.AddMember("Efficiency", temp.fEfficiency, allocator);
		aVillager.AddMember("Wood", temp.iWoodStored, allocator);
		aVillager.AddMember("Stone", temp.iStoneStored, allocator);
		aVillager.AddMember("Food", temp.iFoodStored, allocator);
		aVillager.AddMember("State", temp.eCurrState, allocator);

		Value stats(kArrayType);
			Value aStat(kObjectType);
			aStat.AddMember("Hunting", temp.fStats[Villager::HUNTING], allocator);
			aStat.AddMember("Foraging", temp.fStats[Villager::FORAGING], allocator);
			aStat.AddMember("Woodcutting", temp.fStats[Villager::WOODCUTTING], allocator);
			aStat.AddMember("Building", temp.fStats[Villager::BUILDING], allocator);
			aStat.AddMember("Breeding", temp.fStats[Villager::BREEDING], allocator);
			aStat.AddMember("Combat", temp.fStats[Villager::COMBAT], allocator);
			aStat.AddMember("Mining", temp.fStats[Villager::MINING], allocator);
		stats.PushBack(aStat, allocator);

		aVillager.AddMember("Stats", stats, allocator);

	villagers.PushBack(aVillager, allocator);

	obj.AddMember("Villager", villagers, allocator);

	d.AddMember("Villagers", obj, allocator);
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
	d.Accept(pwriter);
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
	
	FILE* fp = fopen("output.json", "wb"); // non-Windows use "w"
	char writeBuffer[65536];
	FileWriteStream os(fp, writeBuffer, sizeof(writeBuffer));
	PrettyWriter<FileWriteStream> writer(os);
	d.Accept(writer);
	fclose(fp);
	
}