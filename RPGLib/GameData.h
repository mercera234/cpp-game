#pragma once
#include <map>
#include <functional>
#include "BoundInt.h"
#include "Actor.h"
#include "MapRoom.h"
#include "MegaMap.h"
#include "GameItem.h"
#include "EnemyPool.h"
#include "key_strings.h"
#include "Input.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

//null resources
const int nullId = -1;
const chtype nullSymbol = 'X' | COLOR_RED_BOLD << TEXTCOLOR_OFFSET;
const std::string nullName = "-";

class GameData
{
private:
	/*Used to assign a new id to a thing. Once assigned, nextId is incremented. */
	int nextId = 0;

	std::map<std::string, std::string> stringData;
	//Contains all global BoundInts in the game. For some reason this doesn't provide useful data in debugger such as size() ... etc.
	std::map<std::string, BoundInt> bIntData;
	
	std::map<int, Actor> actors;
	std::map<int, MapRoom> mapRooms;
	std::map<int, MegaMap> gameMaps;
	std::map<int, GameItem> gameItems;
	std::multimap<int, EnemyPool> enemyPools; //ids are for map rooms

	std::map<std::string, int> idLookup; //a crossref map for names to ids

	template<typename gameThing, typename repo>
	void insertThing(gameThing& thing, repo& r);

public:
	GameData();
	
	//work with integer data
	void clearData();
	bool storeIntData(const std::string& key, BoundInt value);
	BoundInt& retrieveIntData(const std::string& key);
	bool updateIntData(const std::string& key, int value);
	bool alterIntData(const std::string& key, int amount);

	//get objects

	Actor& getActor(const std::string& name);
	MegaMap& getMap(const std::string& name);
	MapRoom& getRoom(int id);
	MapRoom& getRoom(const std::string& name);
	EnemyPool& getEnemyPool(int id);
	GameItem& getItem(const std::string& name);

	auto& getActors() { return actors; }
	auto& getMaps() { return gameMaps; }
	auto& getRooms() { return mapRooms; }
	auto& getItems() { return gameItems; }
	auto& getEnemyPools() { return enemyPools; }

	//load data from external file routines
	void loadDataFile(const std::string& jsonFile);

	void loadItems(boost::property_tree::ptree& tree);
	void loadActors(boost::property_tree::ptree& tree);
	void loadRooms(boost::property_tree::ptree& tree);
	void loadEnemyPools(boost::property_tree::ptree& tree);
	void loadMaps(boost::property_tree::ptree& tree);
	void loadEverything(boost::property_tree::ptree& jsonFile);


	

	//id routines

	int getNextId() { return nextId++; }
	void resetNextId() { nextId = 0; } //needed for unit testing at least

	/*Load one resource of each type to be used in the absence of other needed resources.
	These resources are generated by the program and do not live on the hard drive.*/
	void loadNullResources();
};

