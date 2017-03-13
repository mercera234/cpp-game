#pragma once
#include "Controllable.h"
#include <list>
using namespace std;
#include "MapMetadata.h"
#include "Map.h"

class MegaMap : public Controllable
{
private:
	list<MapMetadata*>* maps;
	void drawMap(MapMetadata* map);
	chtype bkgd;
public:
	MegaMap(WINDOW* win);
	void addMap(MapMetadata* map);
	void setBkgd(chtype bkgd) { this->bkgd = bkgd; }
	bool removeMap(int mapId);
	bool validate(); //not sure if I need this
	void draw();
};