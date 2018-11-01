#include "Factory.h"
#include "TwoDStorage.h"

MapRoom* Factory::createMap(unsigned short id, unsigned short height, unsigned short width, char patternChar, WINDOW* win)
{
	//create map
	std::string name = "Map " + id;
	MapRoom* newMap = new MapRoom(name, height, width, win);
	//newMap->id = id;

	////populate the map with patternChar
	//Image* display = newMap->getDisplay();

	//ITwoDStorage<chtype>* tileMap = display->getTileMap();

	//for (int i = 0; i < tileMap->getSize(); i++)//set every tenth character to the pattern, leave rest blank
	//{
	//	chtype tile = i % 10 == 0 ? patternChar : ' ';
	//	tileMap->setDatum(i, tile);
	//}


	return newMap;
}

MapRoom* Factory::createMap(WINDOW* win, unsigned short id, unsigned short height, unsigned short width, char patternChar, unsigned short hlY, unsigned short hlX)
{
	//create map
	std::string name = "Map " + id;
	MapRoom* newMap = new MapRoom(name, height, width, win);
	newMap->id = id;

	////populate the map with patternChar
	//Image* display = newMap->getDisplay();

	//ITwoDStorage<chtype>* tileMap = display->getTileMap();

	//for (int i = 0; i < tileMap->getSize(); i++)//set every tenth character to the pattern, leave rest blank
	//{
	//	chtype tile = i % 10 == 0 ? patternChar : ' ';
	//	tileMap->setDatum(i, tile);
	//}

//	newMap->setHighLevelPosition(hlY, hlX);

	return newMap;
}


void Factory::initPaletteMenu(GridMenu& menu, unsigned short rows, unsigned short cols, unsigned short y, unsigned short x)
{
	menu.setCursor(shortCursor);
	menu.setWindow(newwin(rows, cols * 3, y, x));
	menu.setItemWidth(1);
	menu.setFocusable(false);
	menu.setAcceptsMouseInput(true);
	menu.resetItems(rows, cols);
}

LineItem* Factory::createPaletteItem(const std::string& name, chtype icon, unsigned short index)
{
	LineItem* dotItem = new LineItem(name, index, -1);
	dotItem->setField(Fielddraw::DRAW_ICON);
	dotItem->setIcon(icon);
	return dotItem;
}