#include "AutoMap.h"
#include "TUI.h"
#include <map>
#include <sstream>


void AutoMap::setCurrMap(MegaMap* mapIn)
{
	currMap = mapIn;
	reset();
}

void AutoMap::reset()
{
	viewLayer = currMap->getFloorIndex();
}

void AutoMap::setWindow(WINDOW* win)
{
	Controllable::setWindow(win);

	auto& autoMap = currMap->getAutoMap();
	std::for_each(autoMap.begin(), autoMap.end(),
		[win](Image& img) {
		img.setWindow(win);
		img.setBordered(false); //no borders should be drawn around the autoMap
	});
}

void AutoMap::setPosition(int y, int x)
{
	Controllable::setPosition(y, x);

	auto& autoMap = currMap->getAutoMap();
	std::for_each(autoMap.begin(), autoMap.end(),
		[y,x](Image& img) {
		img.setPosition(y,x);
	});
}

void AutoMap::alterViewLayer(int amount)
{
	int temp = viewLayer + currMap->getGroundFloor() + amount;
	if (temp < 0 || temp >= currMap->getDepth())
	{
		return;
	}

	viewLayer += amount;
	return;
}

void AutoMap::processInput()
{
	switch (input)
	{
	case KEY_UP: 
	case KEY_RIGHT:
		alterViewLayer(1);
		break;

	case KEY_DOWN: 
	case KEY_LEFT:
		alterViewLayer(-1);
		break;
	}
}

/*
When AutoMap is visible, it normally won't change even when moving around it
*/
void AutoMap::draw()
{
	//get the curr layer being viewed
	int groundFloorOffset = currMap->getGroundFloor();
	int layerIndex = viewLayer + groundFloorOffset;

	Image& currLayer = currMap->getAutoMap()[layerIndex];

	//draw it centered
	Pos curPos = currMap->getUnitPos();
	int height = getmaxy(win);
	int width = getmaxx(win);
	setPosition(curPos.y - (height / 2), curPos.x - (width / 2));


	currLayer.draw();

	//draw location
	if(viewLayer == currMap->getFloorIndex())
		TUI::printOnBkgd('@' | COLOR_PAIR(COLOR_YELLOW_BOLD), win, curPos.y - ulY, curPos.x - ulX);

	//draw actual floor ontop
	std::string leftArrow = "<-";
	std::string floorLabel = getFloorLabel(viewLayer);
	std::string rightArrow = "->";

	int y = height - 1;
	int x = width - (rightArrow.length() + floorLabel.length());

	//draw <- if there are floors below
	std::ostringstream oss;
	if (groundFloorOffset - currMap->getDepth() < viewLayer)
	{
		oss << leftArrow;
		x -= leftArrow.length();
	}

	oss << floorLabel.c_str();

	//draw -> if there are floors above
	if (currMap->getDepth() - 1 > viewLayer + groundFloorOffset)
	{
		oss << rightArrow;
	}

	mvwaddstr(win, y, x, oss.str().c_str());

	wnoutrefresh(win);
}

//AutoMap::AutoMap(WINDOW* win, int height, int width)
//{
//	/*display = new Image(height, width, win);
//	display->setBordered(false);
//	ITwoDStorage<chtype>& tileMap = display->getTileMap();
//	
//	for (int i = 0; i < display->getTotalTiles(); i++)
//	{
//		tileMap.setDatum(i, ' ');
//	}
//
//	minX = minY = INT32_MAX;
//	maxX = maxY = INT32_MIN;*/
//}

/*
discover an entire map
*/
//void AutoMap::discover(unsigned short mapId)
//{
//	/*MapRoom* map = mapRepo->find(mapId);
//	if (map != NULL)
//		discoverMap(map);*/
//}
//
//void AutoMap::discoverMap(MapRoom* map)
//{
//	/*TwoDStorage<char>* unitMaps = map->getUnitMaps();
//
//	for (unsigned int i = 0; i < unitMaps->getSize(); i++)
//	{
//		char block = unitMaps->getDatum(i);
//		block |= UM_KNOWN;
//		unitMaps->setDatum(i, block);
//	}
//	updateMapInDisplay(map);*/
//}


//void AutoMap::visit(unsigned short mapId, int row, int col)
//{
//	//MapRoom* map = mapRepo->find(mapId);
//	//if (map != NULL)
//	//	visitMap(map, row, col);
//}
//
//
//void AutoMap::visitMap(MapRoom* map, int row, int col)
//{
//	//TwoDStorage<char>* unitMaps = map->getUnitMaps();
//	//char block = unitMaps->getDatum(row, col);
//	//block |= UM_VISITED | UM_KNOWN;
//	//unitMaps->setDatum(row, col, block);
//	//updateMapInDisplay(map);
//}


//update all maps in display
//void AutoMap::updateDisplay()
//{
//	//std::map<unsigned short, MapRoom*>* maps = mapRepo->getMaps();
//
//	//for (std::map<unsigned short, MapRoom*>::iterator it = maps->begin(); it != maps->end(); it++)
//	//{
//	//	MapRoom* map = it->second;
//
//	//	updateMapInDisplay(map);
//	//}
//}


//void AutoMap::updateMapInDisplay(MapRoom* map)
//{
//	//unsigned short endY = map->getPosY() + map->getUnitHeight();
//	//unsigned short endX = map->getPosX() + map->getUnitWidth();
//	//TwoDStorage<chtype>* tileMap = display->getTileMap();
//
//	//int unitMap = 0;
//	//for (int row = map->getPosY(); row < endY; row++)
//	//{
//	//	for (int col = map->getPosX(); col < endX; col++)
//	//	{
//	//		chtype c = ' ';
//
//	//		chtype bkgdColor = (currMap == map->getId()) ? COLOR_CYAN : COLOR_BLUE;
//
//
//	//		char block = map->getUnitMaps()->getDatum(unitMap);
//
//	//		if ((block & UM_VISITED) == UM_VISITED)
//	//			c |= ((bkgdColor | COLOR_BOLD) << BKGDCOLOR_OFFSET);
//	//		else if ((block & UM_KNOWN) == UM_KNOWN)
//	//			c |= (bkgdColor << BKGDCOLOR_OFFSET);
//	//		//not sure if we'll use this scheme or not
//	//		//if (hlm->height == 1 && hlm->width == 1)
//	//		//{
//	//		//	c = '.';
//	//		//}
//	//		//else if (row == hlm->posY && col == hlm->posX) //if upper left corner
//	//		//{
//	//		//	c = '[';
//	//		//}
//	//		//else if (row == endY - 1 && col == endX - 1)
//	//		//{
//	//		//	c = ']';
//	//		//}
//
//	//		c |= (COLOR_GRAY << TEXTCOLOR_OFFSET);
//	//		tileMap->setDatum(row, col, c);
//	//		unitMap++;
//	//	}
//	//}
//}
