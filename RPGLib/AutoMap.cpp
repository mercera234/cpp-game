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

