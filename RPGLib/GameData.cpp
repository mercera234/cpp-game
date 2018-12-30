#include "GameData.h"
#include "KeyComparators.h"

const std::map<std::string, int> colorNameCrossRef =
{
	{ colorNames[0], 0 },
	{ colorNames[1], 1 },
	{ colorNames[2], 2 },
	{ colorNames[3], 3 },
	{ colorNames[4], 4 },
	{ colorNames[5], 5 },
	{ colorNames[6], 6 },
	{ colorNames[7], 7 },
	{ colorNames[8], 8 },
	{ colorNames[9], 9 },
	{ colorNames[10], 10 },
	{ colorNames[11], 11 },
	{ colorNames[12], 12 },
	{ colorNames[13], 13 },
	{ colorNames[14], 14 },
	{ colorNames[15], 15 }
};

GameData::GameData()
{
	loadNullResources();
}


bool GameData::storeIntData(const std::string& key, BoundInt value)
{
	auto retPair = bIntData.insert(std::make_pair(key, value));
	return retPair.second;
}

BoundInt& GameData::retrieveIntData(const std::string& key)
{
	if (bIntData.count(key) > 0)
	{
		return bIntData[key];
	}

	return bIntData[nullName];
}


bool GameData::updateIntData(const std::string& key, int value)
{
	bIntData.at(key).setCurr(value);
	return true;
}


bool GameData::alterIntData(const std::string& key, int amount)
{
	BoundInt& data = bIntData[key];
	data.alterCurr(amount);
	return true;
}

void GameData::clearData()
{
	bIntData.clear();
}

Actor& GameData::getActor(const std::string& name)
{
	int count = idLookup.count(name);
	if (count > 0)
	{
		return actors[idLookup[name]];
	}

	return actors[nullId];
}

MegaMap& GameData::getMap(const std::string& name)
{
	int count = idLookup.count(name);
	if (count > 0)
	{
		return gameMaps[idLookup[name]];
	}

	return gameMaps[nullId];
}

MapRoom& GameData::getRoom(int id)
{
	int count = mapRooms.count(id);
	if (count > 0)
	{
		return mapRooms[id];
	}

	return mapRooms[nullId];
}

MapRoom& GameData::getRoom(const std::string& name)
{
	int count = idLookup.count(name);
	if (count > 0)
	{
		return getRoom(idLookup[name]);
	}

	return mapRooms[nullId];
}

EnemyPool& GameData::getEnemyPool(int id)
{
	int count = enemyPools.count(id);
	if (count > 0)
	{
		return enemyPools.find(id)->second;
	}

	return enemyPools.find(nullId)->second;
}

GameItem& GameData::getItem(const std::string& name)
{
	int count = idLookup.count(name);
	if (count > 0)
	{
		return gameItems[idLookup[name]];
	}

	return gameItems[nullId];
}


void GameData::loadItems(boost::property_tree::ptree& tree)
{
	for each (boost::property_tree::ptree::value_type pair in tree)
	{
		GameItem item;
		char type;

		boost::property_tree::ptree& itemData = pair.second;
		item.name = itemData.get<std::string>("name");
		item.value = itemData.get<int>("value");
		item.cost = itemData.get<int>("cost");

		type = itemData.get<char>("type");
		switch (type)
		{
		case 'M': item.type = GameItemType::MONEY; break;
		case 'C': item.type = GameItemType::CONSUMABLE; break;
		case 'E': item.type = GameItemType::EQUIPPABLE; break;
		case 'K': item.type = GameItemType::KEY; break;
		}
		item.id = getNextId();

		gameItems.insert(std::make_pair(item.id, item));
		idLookup.insert(std::make_pair(item.name, item.id));
	}
}

void GameData::loadActors(boost::property_tree::ptree& tree)
{
	for each (boost::property_tree::ptree::value_type pair in tree)
	{
		Actor actor;

		char symbol;


		boost::property_tree::ptree& actorData = pair.second;

		actor.name = actorData.get<std::string>("name");
		symbol = actorData.get<char>("symbol");
		const std::string color = actorData.get<std::string>("color");

		const int& colorValue = colorNameCrossRef.at(color);

		actor.symbol = symbol | colorValue << TEXTCOLOR_OFFSET;
		actor.getStat(StatType::LEVEL).setCurr(actorData.get<int>("level"));
		actor.getStat(StatType::EXP).setCurr(actorData.get<int>("exp"));
		actor.money.setCurr(actorData.get<int>("money"));

		actor.getStat(StatType::HP).setCurrMax(actorData.get<int>("hp"));
		actor.getStat(StatType::HP).maxOut();
		actor.getStat(StatType::MP).setCurrMax(actorData.get<int>("mp"));
		actor.getStat(StatType::MP).maxOut();

		actor.getStat(StatType::STRENGTH).setCurr(actorData.get<int>("strength"));
		actor.getStat(StatType::DEFENSE).setCurr(actorData.get<int>("defense"));
		actor.getStat(StatType::INTELLIGENCE).setCurr(actorData.get<int>("intelligence"));
		actor.getStat(StatType::WILL).setCurr(actorData.get<int>("will"));
		actor.getStat(StatType::AGILITY).setCurr(actorData.get<int>("agility"));
		actor.id = getNextId();

		actors.insert(std::make_pair(actor.id, actor));
		idLookup.insert(std::make_pair(actor.name, actor.id));
	}

}

void GameData::loadRooms(boost::property_tree::ptree& tree)
{
	for each (boost::property_tree::ptree::value_type pair in tree)
	{
		MapRoom room;

		boost::property_tree::ptree& roomData = pair.second;

		room.setRandomEncounters(roomData.get<bool>("random encounters", true));
		room.setBrightness(roomData.get<bool>("brightness", true));
		room.name = roomData.get<std::string>("name");

		boost::optional<boost::property_tree::ptree&> thingsData = roomData.get_child_optional("things");
		if (thingsData)
		{
			for each (boost::property_tree::ptree::value_type thing in *thingsData)
			{
				boost::property_tree::ptree& thingData = thing.second;
				Sprite* s = new Sprite;
				
				s->thing = &getItem(thingData.get<std::string>("name"));//&gameItems[thingData.get<std::string>("name")];
				s->pos.y = thingData.get<int>("y");
				s->pos.x = thingData.get<int>("x");
				s->quantity = thingData.get<int>("quantity", 1);
				s->height = thingData.get<int>("height", 1);
				s->width = thingData.get<int>("width", 1);
				s->impassible = thingData.get<bool>("impassible", false);
				s->symbol = thingData.get<chtype>("symbol", 0);

				room.sprites.push_back(s);
			}
		}

		room.id = getNextId();

		mapRooms.insert(std::make_pair(room.id, room));
		idLookup.insert(std::make_pair(room.name, room.id));
	}
}

void GameData::loadEnemyPools(boost::property_tree::ptree& tree)
{
	for each (boost::property_tree::ptree::value_type pair in tree)
	{
		EnemyPool pool;

		boost::property_tree::ptree& poolData = pair.second;
		boost::property_tree::ptree& groupData = poolData.get_child("groups");

		for each (boost::property_tree::ptree::value_type groupTree in groupData)
		{
			EnemyGroup group;
			group.weight = groupTree.second.get<int>("weight");

			std::vector<EnemyGroup>& groups = pool.getGroups();
			boost::property_tree::ptree& enemyData = groupTree.second.get_child("enemies");

			for each (boost::property_tree::ptree::value_type enemyTree in enemyData)
			{
				group.enemyNames.push_back(enemyTree.second.get_value<std::string>());
			}

			groups.push_back(group);
		}

		boost::property_tree::ptree& roomData = poolData.get_child("rooms");

		for each (boost::property_tree::ptree::value_type roomTree in roomData)
		{
			std::string roomName = roomTree.second.get_value<std::string>();

			int roomId = idLookup[roomName];

			enemyPools.insert(std::make_pair(roomId, pool));
		}
	}
}

void GameData::loadMaps(boost::property_tree::ptree& tree)
{
	for each (boost::property_tree::ptree::value_type pair in tree)
	{
		MegaMap map;
		boost::property_tree::ptree& mapData = pair.second;

		map.name = mapData.get<std::string>("name");
		map.setDimensions(mapData.get<int>("rows"), mapData.get<int>("cols"), mapData.get<int>("floors", 1));
		map.setGroundFloor(mapData.get<int>("ground floor"));

		Image* image = new Image[map.getDepth()];

		//swap real map ids for crossreferenced ones
		boost::property_tree::ptree& crossRefs = mapData.get_child("crossRef");
		std::map<int, int> idCrossRef;
		for (auto it = crossRefs.begin(); it != crossRefs.end(); it++)
		{
			boost::property_tree::ptree& crossRefNode = (*it).second;
			std::string name = crossRefNode.get<std::string>("name");
			int index = crossRefNode.get<int>("index");

			int roomId = idLookup[name];
			idCrossRef.insert(std::make_pair(index, roomId));
		}


		boost::property_tree::ptree& roomIds = mapData.get_child("data");
		boost::property_tree::ptree::iterator it = roomIds.begin();
		for (int floor = map.getDepth() - 1; floor >= 0; floor--)
		{
			image[floor].setDimensions(map.getUnitRows(), map.getUnitCols());
			image[floor].getTileMap().fill(nullId);

			for (int row = 0; row < map.getUnitRows(); row++)
			{
				for (int col = 0; col < map.getUnitCols(); col++)
				{
					int c = (*it).second.get_value<int>();
					int actualId = c < 0 ? c : idCrossRef[c];
					image[floor].setTile(row, col, actualId);
					it++;
				}
			}

			map.setLayerImage(map.getFloorFromIndex(floor), image[floor]);
		}



		map.id = getNextId();
		gameMaps.insert(std::make_pair(map.id, map));
		idLookup.insert(std::make_pair(map.name, map.id));

		delete[] image;
	}
}

void GameData::loadEverything(boost::property_tree::ptree& jsonFile)
{
	loadItems(jsonFile.get_child("items"));
	loadActors(jsonFile.get_child("actors"));
	loadRooms(jsonFile.get_child("rooms"));
	loadMaps(jsonFile.get_child("maps"));
	loadEnemyPools(jsonFile.get_child("enemy pools")); //must be loaded after the rooms
}

void GameData::loadDataFile(const std::string& jsonFile)
{
	namespace pt = boost::property_tree;
	pt::ptree root;
	pt::read_json(jsonFile, root); //reading in the file also validates it

	loadEverything(root);
}


void GameData::loadNullResources()
{
	//load null bound int
	BoundInt nullBoundInt(0,0);
	storeIntData(nullName, nullBoundInt);

	//load null item
	GameItem nullItem;
	nullItem.id = nullId;
	nullItem.name = nullName;
	gameItems.insert(std::make_pair(nullId, nullItem));

	//load null actor
	Actor nullActor;
	nullActor.symbol = nullSymbol;
	nullActor.name = nullName;

	actors.insert(std::make_pair(nullId, nullActor));

	//load null map
	MapRoom nullRoom;
	nullRoom.setDimensions(getScreenHeight(), getScreenWidth());
	Image& img = nullRoom.getDisplay();
	ITwoDStorage<chtype>& tiles = img.getTileMap();

	for (int row = 0; row < getScreenHeight(); row++)
	{
		for (int col = 0; col < getScreenWidth(); col++)
		{
			char c = (row % 4 == 0 && col % 8 == 0) ? '!' : ' ';
			tiles.setDatum(row, col, c);
		}
	}

	mapRooms.insert(std::make_pair(nullId, nullRoom));

	//load null megamap
	MegaMap nullMap;
	nullMap.setDimensions(1, 1);

	Image layout;
	layout.setDimensions(1, 1);
	layout.setTile(0, 0, nullId);
	nullMap.setLayerImage(0, layout);

	gameMaps.insert(std::make_pair(nullId, nullMap));

	//load null enemyPool
	EnemyPool nullPool;
	enemyPools.insert(std::make_pair(nullId, nullPool));
}