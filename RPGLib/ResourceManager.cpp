#include "ResourceManager.h"
#include "KeyComparators.h"
#include "key_strings.h"

const char commentRecordId = '#';

ResourceManager::ResourceManager() : actors(stringCompare)
{
	loadNullResources();
}


int ResourceManager::loadActorsFromTextFile(std::ifstream& textFile)
{
	int loaded = actors.size();

	char lineFirstChar;
	while ((lineFirstChar = (char)textFile.peek()) != EOF)
	{
		if (lineFirstChar == commentRecordId) //skip to next line
		{
			//see max conflict with windef.h, hence the extra ()s
			textFile.ignore((std::numeric_limits<int>::max)(), '\n');
			continue;
		}

		Actor actor;

		char symbol, color;
		int level, exp, money, hp, mp, str, def, intl, wil, agi;

		textFile >> actor.name >> symbol >> color >> level >> exp >> money >> hp >> mp >> str >> def >> intl >> wil >> agi;

		fixString(actor.name);
		
		actor.symbol = symbol | getColor(color) << TEXTCOLOR_OFFSET;
		actor.getStat(StatType::LEVEL).setCurr(level);
		actor.getStat(StatType::EXP).setCurr(exp);
		actor.money.setCurr(money);

		actor.getStat(StatType::HP).setCurrMax(hp);
		actor.getStat(StatType::HP).maxOut();
		actor.getStat(StatType::MP).setCurrMax(mp);
		actor.getStat(StatType::MP).maxOut();

		actor.getStat(StatType::STRENGTH).setCurr(str);
		actor.getStat(StatType::DEFENSE).setCurr(def);
		actor.getStat(StatType::INTELLIGENCE).setCurr(intl);
		actor.getStat(StatType::WILL).setCurr(wil);
		actor.getStat(StatType::AGILITY).setCurr(agi);
		actor.id = getNextId();

		actors.insert(std::make_pair(actor.name, actor));
	}

	return actors.size() - loaded;
}

void ResourceManager::fixString(std::string& s)
{
	std::replace(s.begin(), s.end(), '_', ' '); //This handles any multi-word names
}

int ResourceManager::getColor(char c)
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

int ResourceManager::loadGameMapsFromDir(FileDirectory& dir)
{
	int loaded = gameItems.size();
	//get list of map files
	std::string filter = "map";
	std::list<dirent> files = dir.getFiles(false, filter);

	for each (dirent file in files)
	{
		MapRoom gameMap;

		std::ifstream is(dir.getPath() + '\\' + file.d_name);
		gameMap.load(is);

		gameMap.setId(getNextId());
		mapRooms.insert(std::make_pair(gameMap.getId(), gameMap)); //id is set automatically not manually
	}

	return mapRooms.size() - loaded;
}

int ResourceManager::loadMapRoomsFromTextFile(std::ifstream& textFile)
{
	int loaded = mapRooms.size();

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

		room.setName(name);
		room.setBrightness(brightness);
		room.setRandomEncounters(randomEncounters);
		room.setId(getNextId());
		//so far this loads the map metadata, but not the image, effectslayer, overlays, or items

		mapRooms.insert(std::make_pair(room.getId(), room));
	}

	return mapRooms.size() - loaded;
}


int ResourceManager::loadMapsFromTextFile(std::ifstream& textFile)
{
	/*TODO function is setup to read in map data from file, this should come from separate image file identified in the map text file
	Currently unable to edit MegaMaps
	*/
	int loaded = gameMaps.size();

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
	

	gameMaps.insert(std::make_pair(map.name, map));

	delete [] image;

	return gameMaps.size() - loaded;
}


int ResourceManager::loadItemsFromTextFile(std::ifstream& textFile)
{
	int loaded = gameItems.size();

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
		case 'C': item.type = GameItemType::CONSUMABLE;
		case 'E': item.type = GameItemType::EQUIPPABLE;
		case 'K': item.type = GameItemType::KEY;
		}
		fixString(item.name);

		gameItems.insert(std::make_pair(item.name, item));
	}

	return gameItems.size() - loaded;
}

int ResourceManager::loadConfigurationFile(std::ifstream& textFile)
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

		inputs.insert(std::make_pair(key, Input(textInput, -1)));
	}

	return inputs.size();
}

void ResourceManager::loadNullResources()
{
	//load null actor
	Actor nullActor;
	nullActor.symbol = nullSymbol;
	nullActor.name = nullName;

	actors.insert(std::make_pair(nullActor.name, nullActor));

	//load null map
	MapRoom nullRoom;
	nullRoom.setDimensions(screenHeight, screenWidth);
	Image* img = nullRoom.getDisplay();
	TwoDStorage<chtype>* tiles = img->getTileMap();

	for (int row = 0; row < screenHeight; row++)
	{
		for (int col = 0; col < screenWidth; col++)
		{
			char c = (row % 4 == 0 && col % 8 == 0) ? '!' : ' ';
			tiles->setDatum(row, col, c);
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

	gameMaps.insert(std::make_pair(nullName, nullMap));
}



