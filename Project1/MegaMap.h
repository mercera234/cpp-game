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
	//the height/width of a unit map. All maps loaded must have dimensions that are a multiple of the unit map size
	unsigned short unitsHigh;
	unsigned short unitsWide;

	//high level coordinates of player in megamap
	unsigned short blockX;
	unsigned short blockY;
	

	MegaMap(WINDOW* win, unsigned short unitsHigh, unsigned short unitsWide);
	void addMap(MapMetadata* map);
	// void addMap(Map* map, int hY, int hX, short layer); //this can create a metadata object automatically


	Map* findMap(unsigned short hY, unsigned short hX);
	void setCurrMap(Map* map) { currMap = map; }
	Map* getCurrMap() { return currMap; }
	void setBkgd(chtype bkgd) { this->bkgd = bkgd; }
	bool removeMap(int mapId);
	bool validate(); //not sure if I need this
	void draw();
};