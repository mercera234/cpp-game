#pragma once
#include "Map.h"
#include "GridMenu.h"
#include "LineItem.h"

class Factory
{
private:

public:
	Map* createMap(unsigned short id, unsigned short height, unsigned short width, char patternChar, WINDOW* win); //should maybe get rid of this one
	Map* createMap(WINDOW* win, unsigned short id, unsigned short height, unsigned short width, char patternChar, unsigned short hlY, unsigned short hlX);
	void initPaletteMenu(GridMenu& menu, unsigned short rows, unsigned short cols, unsigned short y, unsigned short x);
	LineItem* createPaletteItem(const std::string& name, chtype icon, unsigned short index);
};
