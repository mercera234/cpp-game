#include "ResourceManager.h"
#include "KeyComparators.h"
#include "key_strings.h"

ResourceManager::ResourceManager()
{
	loadNullResources();
}

Actor& ResourceManager::getActor(const std::string& name)
{
	int count = actors.count(name);
	if (count > 0)
	{
		return actors[name];
	}

	return actors[nullName];
}

MegaMap& ResourceManager::getMap(const std::string& name)
{
	int count = gameMaps.count(name);
	if (count > 0)
	{
		return gameMaps[name];
	}

	return gameMaps[nullName];
}

MapRoom& ResourceManager::getRoom(int id)
{
	int count = mapRooms.count(id);
	if (count > 0)
	{
		return mapRooms[id];
	}

	return mapRooms[nullId];
}

Actor* ResourceManager::getPlayer1()
{
	if (playerParty.empty())
		return nullptr;

	return playerParty.front();
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

		gameMap.id = getNextId();
		mapRooms.insert(std::make_pair(gameMap.id, gameMap)); //id is set automatically not manually
	}

	return mapRooms.size() - loaded;
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
	nullRoom.setDimensions(getScreenHeight(), getScreenWidth());
	Image* img = nullRoom.getDisplay();
	TwoDStorage<chtype>* tiles = img->getTileMap();

	for (int row = 0; row < getScreenHeight(); row++)
	{
		for (int col = 0; col < getScreenWidth(); col++)
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



