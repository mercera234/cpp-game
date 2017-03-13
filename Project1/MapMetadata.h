#pragma once
#include "2DStorage.h"

struct UnitMap
{
	bool visited;
};

struct MapMetadata
{
	int hY;
	int hX;
	int mapId;
	short layer;
	unsigned short unitWidth;
	unsigned short unitHeight;
	char visualId;
	_2DStorage<UnitMap>* units;
};