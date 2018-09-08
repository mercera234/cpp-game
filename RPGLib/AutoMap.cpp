#include "AutoMap.h"
#include "TUI.h"
#include <map>

AutoMap::AutoMap(WINDOW* win, int height, int width)
{
	display = new Image(height, width, win);
	display->setBordered(false);
	TwoDStorage<chtype>* tileMap = display->getTileMap();
	
	for (unsigned int i = 0; i < display->getTotalTiles(); i++)
	{
		tileMap->setDatum(i, ' ');
	}

	minX = minY = INT32_MAX;
	maxX = maxY = INT32_MIN;
}


//update all maps in display
void AutoMap::updateDisplay()
{
	std::map<unsigned short, MapRoom*>* maps = mapRepo->getMaps();

	for (std::map<unsigned short, MapRoom*>::iterator it = maps->begin(); it != maps->end(); it++)
	{
		MapRoom* map = it->second;

		updateMapInDisplay(map);
	}
}


void AutoMap::updateMapInDisplay(MapRoom* map)
{
	//unsigned short endY = map->getPosY() + map->getUnitHeight();
	//unsigned short endX = map->getPosX() + map->getUnitWidth();
	//TwoDStorage<chtype>* tileMap = display->getTileMap();

	//int unitMap = 0;
	//for (int row = map->getPosY(); row < endY; row++)
	//{
	//	for (int col = map->getPosX(); col < endX; col++)
	//	{
	//		chtype c = ' ';

	//		chtype bkgdColor = (currMap == map->getId()) ? COLOR_CYAN : COLOR_BLUE;


	//		char block = map->getUnitMaps()->getDatum(unitMap);

	//		if ((block & UM_VISITED) == UM_VISITED)
	//			c |= ((bkgdColor | COLOR_BOLD) << BKGDCOLOR_OFFSET);
	//		else if ((block & UM_KNOWN) == UM_KNOWN)
	//			c |= (bkgdColor << BKGDCOLOR_OFFSET);
	//		//not sure if we'll use this scheme or not
	//		//if (hlm->height == 1 && hlm->width == 1)
	//		//{
	//		//	c = '.';
	//		//}
	//		//else if (row == hlm->posY && col == hlm->posX) //if upper left corner
	//		//{
	//		//	c = '[';
	//		//}
	//		//else if (row == endY - 1 && col == endX - 1)
	//		//{
	//		//	c = ']';
	//		//}

	//		c |= (COLOR_GRAY << TEXTCOLOR_OFFSET);
	//		tileMap->setDatum(row, col, c);
	//		unitMap++;
	//	}
	//}
}


/*
When AutoMap is visible, it normally won't change even when moving around it
*/
void AutoMap::draw()
{
	//draw image
	display->draw();
}


/*
discover an entire map
*/
void AutoMap::discover(unsigned short mapId)
{
	MapRoom* map = mapRepo->find(mapId);
	if (map != NULL)
		discoverMap(map);
}

void AutoMap::discoverMap(MapRoom* map)
{
	/*TwoDStorage<char>* unitMaps = map->getUnitMaps();

	for (unsigned int i = 0; i < unitMaps->getSize(); i++)
	{
		char block = unitMaps->getDatum(i);
		block |= UM_KNOWN;
		unitMaps->setDatum(i, block);
	}
	updateMapInDisplay(map);*/
}


void AutoMap::visit(unsigned short mapId, int row, int col)
{
	MapRoom* map = mapRepo->find(mapId);
	if (map != NULL)
		visitMap(map, row, col);
}


void AutoMap::visitMap(MapRoom* map, int row, int col)
{
	//TwoDStorage<char>* unitMaps = map->getUnitMaps();
	//char block = unitMaps->getDatum(row, col);
	//block |= UM_VISITED | UM_KNOWN;
	//unitMaps->setDatum(row, col, block);
	//updateMapInDisplay(map);
}


void AutoMap::setCurrMap(unsigned short mapId)
{
	int oldCurrMap = currMap;
	currMap = mapId;

	MapRoom* map = mapRepo->find(oldCurrMap);
	if (map != NULL)
		updateMapInDisplay(map);
}