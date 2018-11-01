#include "data_loaders.h"
#include "key_strings.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

const char commentRecordId = '#';


const std::map<std::string, int> colorNameCrossRef = 
{
	{ colorNames[0], 0},
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

/*Replace underscores with spaces.
Strings have to be stored in text file with underscores or else they will not be parsed correctly.*/
void fixString(std::string& s)
{
	std::replace(s.begin(), s.end(), '_', ' '); //This handles any multi-word names
}

int getColor(char c)
{
	int color = 0;
	switch (c)
	{
	case 'c': color = COLOR_CYAN; break;
	case 'C': color = COLOR_CYAN_BOLD; break;
	case 'g': color = COLOR_GREEN; break;
	case 'G': color = COLOR_GREEN_BOLD; break;
	case 'r': color = COLOR_RED; break;
	case 'R': color = COLOR_RED_BOLD; break;
	case 'b': color = COLOR_BLUE; break;
	case 'B': color = COLOR_BLUE_BOLD; break;
	case 'y': color = COLOR_YELLOW; break;
	case 'Y': color = COLOR_YELLOW_BOLD; break;
	case 'w': color = COLOR_WHITE; break;
	case 'W': color = COLOR_WHITE_BOLD; break;
	case 'a':
	case 'A':
	case 'K': color = COLOR_GRAY; break;
	case 'k': color = COLOR_BLACK; break;
	}

	return color;
}



int loadConfigurationFile(std::ifstream& textFile, ResourceManager& rm)
{
	char lineFirstChar;
	while ((lineFirstChar = (char)textFile.peek()) != EOF)
	{
		if (lineFirstChar == commentRecordId) //skip to next line
		{
			//see max conflict with windef.h, hence the extra ()s
			textFile.ignore((std::numeric_limits<int>::max)(), '\n');
			continue;
		}

		std::string textInput;
		std::string textKey;

		textFile >> textInput >> textKey;
		int key = getKeyFromString(textKey);

		rm.inputs.insert(std::make_pair(key, Input(textInput, -1)));
	}

	return rm.inputs.size();
}



void loadItems(boost::property_tree::ptree& tree, ResourceManager& rm)
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
		item.id = rm.getNextId();

		rm.gameItems.insert(std::make_pair(item.name, item));
		rm.idLookup.insert(std::make_pair(item.name, item.id));
	}
}

void loadActors(boost::property_tree::ptree& tree, ResourceManager& rm)
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
		actor.id = rm.getNextId();

		rm.actors.insert(std::make_pair(actor.name, actor));
		rm.idLookup.insert(std::make_pair(actor.name, actor.id));
	}
	
}

void loadRooms(boost::property_tree::ptree& tree, ResourceManager& rm)
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
				s->thing = &rm.gameItems[thingData.get<std::string>("name")];
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
		
		room.id = rm.getNextId();

		rm.mapRooms.insert(std::make_pair(room.id, room));
		rm.idLookup.insert(std::make_pair(room.name, room.id));
	}
}

void loadEnemyPools(boost::property_tree::ptree& tree, ResourceManager& rm)
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

			int roomId = rm.idLookup[roomName];

			rm.enemyPools.insert(std::make_pair(roomId, pool));
		}
	}
}

void loadMaps(boost::property_tree::ptree& tree, ResourceManager& rm)
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

			int roomId = rm.idLookup[name];
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

		

		map.id = rm.getNextId();
		rm.gameMaps.insert(std::make_pair(map.id, map));
		rm.idLookup.insert(std::make_pair(map.name, map.id));

		delete[] image;
	}
}

void loadEverything(boost::property_tree::ptree& jsonFile, ResourceManager& rm)
{
	loadItems(jsonFile.get_child("items"), rm);
	loadActors(jsonFile.get_child("actors"), rm);
	loadRooms(jsonFile.get_child("rooms"), rm);
	loadMaps(jsonFile.get_child("maps"), rm);
	loadEnemyPools(jsonFile.get_child("enemy pools"), rm); //must be loaded after the rooms
}

void loadDataFile(const std::string& jsonFile, ResourceManager& rm)
{
	namespace pt = boost::property_tree;
	pt::ptree root;
	pt::read_json(jsonFile, root); //reading in the file also validates it

	loadEverything(root, rm);
}


