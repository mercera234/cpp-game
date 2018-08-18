#pragma once
#include "TwoDStorage.h"
#include "Map.h"

struct UnitMap
{
	bool visited;
};

struct MapMetadata
{
	//id the specific map
	unsigned short mapId;
	Map* map;

	//dimensional data
	//x and y coordinates of map upper left corner when viewed in automap mode. A high level set of coordinates
	int hY;
	int hX;
	short layer; //represents the floor of the megamap. It's all relative, but 0 could be thought of as ground level

	//unit width and height of this map
	unsigned short unitsWide;
	unsigned short unitsTall;
	

	
	char visualId; //a character used to help distinguish one map within the megamap from the adjoining maps
	TwoDStorage<UnitMap>* units;
};

//discovered (submap): true - submap has been visited and is showing on automap (dark) | false - submap is not visible at all
//visited (mapblock): true - submap has been visited and is shown on automap (bold) | false - submap block is either dark or not visible at all
//walking into a map discovers it. Walking on a specific mapblock visits it.
//a map item can also discover parts of the automap 