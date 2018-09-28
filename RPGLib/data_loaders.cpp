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


//void loadActorFromTextFile(std::ifstream& textFile, ResourceManager& rm)
//{
//	Actor actor;
//
//	char symbol, color;
//	int level, exp, money, hp, mp, str, def, intl, wil, agi;
//
//	textFile >> actor.name >> symbol >> color >> level >> exp >> money >> hp >> mp >> str >> def >> intl >> wil >> agi;
//
//	fixString(actor.name);
//
//	actor.symbol = symbol | getColor(color) << TEXTCOLOR_OFFSET;
//	actor.getStat(StatType::LEVEL).setCurr(level);
//	actor.getStat(StatType::EXP).setCurr(exp);
//	actor.money.setCurr(money);
//
//	actor.getStat(StatType::HP).setCurrMax(hp);
//	actor.getStat(StatType::HP).maxOut();
//	actor.getStat(StatType::MP).setCurrMax(mp);
//	actor.getStat(StatType::MP).maxOut();
//
//	actor.getStat(StatType::STRENGTH).setCurr(str);
//	actor.getStat(StatType::DEFENSE).setCurr(def);
//	actor.getStat(StatType::INTELLIGENCE).setCurr(intl);
//	actor.getStat(StatType::WILL).setCurr(wil);
//	actor.getStat(StatType::AGILITY).setCurr(agi);
//	actor.id = rm.getNextId();
//
//	rm.actors.insert(std::make_pair(actor.name, actor));
//}


//int loadActorsFromTextFile(std::ifstream& textFile, ResourceManager& rm)
//{
//	int loaded = rm.actors.size();
//
//	char lineFirstChar;
//	while ((lineFirstChar = (char)textFile.peek()) != EOF)
//	{
//		if (lineFirstChar == commentRecordId) //skip to next line
//		{
//			//see max conflict with windef.h, hence the extra ()s
//			textFile.ignore((std::numeric_limits<int>::max)(), '\n');
//			continue;
//		}
//
//		loadActorFromTextFile(textFile, rm);
//
//	}
//
//	return rm.actors.size() - loaded;
//}

int loadMapRoomsFromTextFile(std::ifstream& textFile, ResourceManager& rm)
{
	int loaded = rm.mapRooms.size();

	char lineFirstChar;
	while ((lineFirstChar = (char)textFile.peek()) != EOF)
	{
		if (lineFirstChar == commentRecordId) //skip to next line
		{
			//see max conflict with windef.h, hence the extra ()s
			textFile.ignore((std::numeric_limits<int>::max)(), '\n');
			continue;
		}

		MapRoom room;

		bool brightness;
		bool randomEncounters;
		std::string name;
		std::string imagePath;

		textFile >> name >> imagePath >> brightness >> randomEncounters;

		fixString(name);

		room.name = name;
		room.setBrightness(brightness);
		room.setRandomEncounters(randomEncounters);
		room.id = rm.getNextId();
		//so far this loads the map metadata, but not the image, effectslayer, overlays, or items

		rm.mapRooms.insert(std::make_pair(room.id, room));
	}

	return rm.mapRooms.size() - loaded;
}


int loadMapsFromTextFile(std::ifstream& textFile, ResourceManager& rm)
{
	/*TODO function is setup to read in map data from file, this should come from separate image file identified in the map text file
	Currently unable to edit MegaMaps
	*/
	int loaded = rm.gameMaps.size();

	MegaMap map;

	Image* image = nullptr;
	int currRow = 0;

	char lineFirstChar;
	while ((lineFirstChar = (char)textFile.peek()) != EOF)
	{
		switch (lineFirstChar)
		{
		case commentRecordId:  //skip to next line
							   //see max conflict with windef.h, hence the extra ()s
			textFile.ignore((std::numeric_limits<int>::max)(), '\n');
			continue;
		case 'D':
		{
			textFile.ignore(1, ' ');
			int rows, cols;
			int floors, groundFloor;
			textFile >> map.name >> rows >> cols >> floors >> groundFloor;
			fixString(map.name);
			map.setDimensions(rows, cols, floors);

			image = new Image[floors];

			for (int i = 0; i < floors; i++)
			{
				image[i].setDimensions(rows, cols);
				image[i].getTileMap()->fill(INT_MAX);
			}
			map.setGroundFloor(groundFloor);
		}
		break;
		case 'R':
		{
			textFile.ignore(1, ' ');

			int floor;
			textFile >> floor;

			floor = map.getRealIndexFromFloor(floor);
			for (int i = 0; i < map.getUnitCols(); i++)
			{
				chtype c;
				textFile >> c;
				image[floor].setTile(currRow, i, c);
			}
			currRow++;
			currRow %= map.getUnitRows();
		}
		break;
		default:
			textFile.get();//advance one character
		}
	}

	for (int i = 0; i < map.getDepth(); i++)
	{
		map.setLayerImage(map.getFloorFromIndex(i), image[i]);
	}


	rm.gameMaps.insert(std::make_pair(map.name, map));

	delete[] image;

	return rm.gameMaps.size() - loaded;
}


int loadItemsFromTextFile(std::ifstream& textFile, ResourceManager& rm)
{
	int loaded = rm.gameItems.size();

	char lineFirstChar;
	while ((lineFirstChar = (char)textFile.peek()) != EOF)
	{
		if (lineFirstChar == commentRecordId) //skip to next line
		{
			//see max conflict with windef.h, hence the extra ()s
			textFile.ignore((std::numeric_limits<int>::max)(), '\n');
			continue;
		}

		GameItem item;
		char type;

		textFile >> item.name >> type >> item.cost >> item.value;

		switch (type)
		{
		case 'M': item.type = GameItemType::MONEY;
		case 'C': item.type = GameItemType::CONSUMABLE;
		case 'E': item.type = GameItemType::EQUIPPABLE;
		case 'K': item.type = GameItemType::KEY;
		}
		fixString(item.name);

		rm.gameItems.insert(std::make_pair(item.name, item));
	}

	return rm.gameItems.size() - loaded;
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

void loadEnemyPools(boost::property_tree::ptree& tree, ResourceManager& rm)
{
	for each (boost::property_tree::ptree::value_type pair in tree)
	{
		EnemyPool pool;

		boost::property_tree::ptree& poolData = pair.second;
		std::string mapName = poolData.get<std::string>("map");
		
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

		rm.enemyPools.insert(std::make_pair(mapName, pool));
	}
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

		rm.gameItems.insert(std::make_pair(item.name, item));
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
	}
	
}


void loadDataFile(const std::string& jsonFile, ResourceManager& rm)
{
	namespace pt = boost::property_tree;
	pt::ptree root;
	pt::read_json(jsonFile, root); //reading in the file also validates it

	loadItems(root.get_child("items"), rm);
	loadActors(root.get_child("actors"), rm);
	loadEnemyPools(root.get_child("enemy pools"), rm);
	
	
}

