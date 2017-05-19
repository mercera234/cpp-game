#pragma once
#include "Controllable.h"
#include <list>
using namespace std;
#include "MapMetadata.h"
#include "Map.h"

class MegaMap : public Controllable
{
private:
	list<MapMetadata*> maps;
	void drawMap(MapMetadata* map);
	chtype bkgd;
	Map* currMap;

public:
	//high level coordinates of player in megamap
	unsigned short hX;
	unsigned short hY;
	//the height/width of a unit map. All maps loaded must have dimensions that are a multiple of the unit map size
	unsigned short unitHeight;
	unsigned short unitWidth;

	MegaMap(WINDOW* win, unsigned short unitHeight, unsigned short unitWidth);
	void addMap(MapMetadata* map);
	Map* findMap(unsigned short hY, unsigned short hX);
	void setCurrMap(Map* map) { currMap = map; }
	Map* getCurrMap() { return currMap; }
	void setBkgd(chtype bkgd) { this->bkgd = bkgd; }
	bool removeMap(int mapId);
	bool validate(); //not sure if I need this
	void draw();
};