#include "Factory.h"
#include "2DStorage.h"

Map* Factory::createMap(unsigned short id, unsigned short height, unsigned short width, char patternChar, WINDOW* win)
{
	//create map
	string name = "Map " + id;
	Map* newMap = new Map(name, height, width, win);
	newMap->setId(id);

	//populate the map with patternChar
	Image* display = newMap->getDisplay();

	_2DStorage<chtype>* tileMap = display->getTileMap();

	for (int i = 0; i < tileMap->getSize(); i++)//set every tenth character to the pattern, leave rest blank
	{
		chtype tile = i % 10 == 0 ? patternChar : ' ';
		tileMap->setDatum(i, tile);
	}


	return newMap;
}

Map* Factory::createMap(WINDOW* win, unsigned short id, unsigned short height, unsigned short width, char patternChar, unsigned short hlY, unsigned short hlX)
{
	//create map
	string name = "Map " + id;
	Map* newMap = new Map(name, height, width, win);
	newMap->setId(id);

	//populate the map with patternChar
	Image* display = newMap->getDisplay();

	_2DStorage<chtype>* tileMap = display->getTileMap();

	for (int i = 0; i < tileMap->getSize(); i++)//set every tenth character to the pattern, leave rest blank
	{
		chtype tile = i % 10 == 0 ? patternChar : ' ';
		tileMap->setDatum(i, tile);
	}

	newMap->setHighLevelPosition(hlY, hlX);

	return newMap;
}