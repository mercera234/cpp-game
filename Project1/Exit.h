#pragma once

struct MapExit
{
	int destMapId;
	MapExit(int id) { destMapId = id; }
};

struct InMapExit : public MapExit
{
	int destY;
	int destX;

	InMapExit(int y, int x, int id) : MapExit(id)
	{ destY = y; destX = x; }
};

#define B_NORTH 0
#define B_SOUTH 1
#define B_WEST  2
#define B_EAST  3

struct BorderExit : public MapExit
{
	int edge;
	int start;
	int length;
	BorderExit(int edge, int start, int length, int id) : MapExit(id)
	{
		this->edge = edge; 
		this->start = start;
		this->length = length;
	}
};