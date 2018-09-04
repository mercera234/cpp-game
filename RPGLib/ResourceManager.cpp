#include "ResourceManager.h"
#include "KeyComparators.h"

ResourceManager::ResourceManager() : actors(stringCompare)
{
	
}

int ResourceManager::loadActorsFromTextFile(std::ifstream& textFile)
{
	char lineFirstChar;
	while ((lineFirstChar = (char)textFile.peek()) != EOF)
	{
		if (lineFirstChar == '#') //skip to next line
		{
			//see max conflict with windef.h, hence the extra ()s
			textFile.ignore((std::numeric_limits<int>::max)(), '\n');
			continue;
		}

		Actor actor;

		char symbol, color;
		int level, exp, money, hp, mp, str, def, intl, wil, agi;

		textFile >> actor.name >> symbol >> color >> level >> exp >> money >> hp >> mp >> str >> def >> intl >> wil >> agi;

		std::replace(actor.name.begin(), actor.name.end(), '_', ' '); //This handles any multi-word names

		actor.symbol = symbol | getColor(color) << TEXTCOLOR_OFFSET;
		actor.getStat(StatType::LEVEL).setCurr(level);
		actor.getStat(StatType::EXP).setCurr(exp);
		actor.getStat(StatType::MONEY).setCurr(money);

		actor.getStat(StatType::HP).setCurrMax(hp);
		actor.getStat(StatType::HP).maxOut();
		actor.getStat(StatType::MP).setCurrMax(mp);
		actor.getStat(StatType::MP).maxOut();

		actor.getStat(StatType::STRENGTH).setCurr(str);
		actor.getStat(StatType::DEFENSE).setCurr(def);
		actor.getStat(StatType::INTELLIGENCE).setCurr(intl);
		actor.getStat(StatType::WILL).setCurr(wil);
		actor.getStat(StatType::AGILITY).setCurr(agi);

		actors.insert(std::make_pair(actor.name, actor));
	}

	return actors.size();
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
	//get list of map files
	std::string filter = "map";
	std::list<dirent> files = dir.getFiles(false, filter);

	int id = 0;
	for each (dirent file in files)
	{
		Map gameMap;
		gameMap.load(dir.getPath() + '\\' + file.d_name);

		gameMap.setId(id);
		gameMaps.insert(std::make_pair(id++, gameMap)); //id is set automatically not manually
	}

	return gameMaps.size();
}

void ResourceManager::loadNullResources()
{
	//load null actor
	Actor nullActor;
	nullActor.symbol = nullSymbol;
	nullActor.name = nullName;

	actors.insert(std::make_pair(nullActor.name, nullActor));

	//load null map
	Map nullMap;
	nullMap.setDimensions(screenHeight, screenWidth);
	Image* img = nullMap.getDisplay();
	TwoDStorage<chtype>* tiles = img->getTileMap();

	for (int row = 0; row < screenHeight; row++)
	{
		for (int col = 0; col < screenWidth; col++)
		{
			char c = (row % 4 == 0 && col % 8 == 0) ? '!' : ' ';
			tiles->setDatum(row, col, c);
		}
	}

	gameMaps.insert(std::make_pair(-1, nullMap));

}
