#pragma once
#include "2DStorage.h"
#include "Map.h"

struct UnitMap
{
	bool visited;
};

struct MapMetadata
{
	//x and y coordinates when viewed in automap mode. A high level set of coordinates
	int hY;
	int hX;

	int mapId;
	Map* map;

	short layer; //represents the floor of the megamap

	//unit width and height of this map
	unsigned short unitsWide;  
	unsigned short unitsTall; 
	char visualId; //a character used to help distinguish one map within the megamap from the adjoining maps
	_2DStorage<UnitMap>* units;
};