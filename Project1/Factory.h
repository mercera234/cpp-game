#pragma once
#include "Map.h"

class Factory
{
private:

public:
	Map* createMap(unsigned short id, unsigned short height, unsigned short width, char patternChar, WINDOW* win); //should maybe get rid of this one
	Map* createMap(WINDOW* win, unsigned short id, unsigned short height, unsigned short width, char patternChar, unsigned short hlY, unsigned short hlX);
};
