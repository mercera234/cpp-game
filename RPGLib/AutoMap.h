#pragma once
#include "MapRepository.h"
#include <vector>
#include <list>

#define UM_KNOWN 0x01 //this should maybe be part of the hilevel map structure
#define UM_VISITED 0x02

/*A control used for viewing a high-level map.
Will display tiles differently depending on whether they are known or visited.*/
class AutoMap : public Controllable
{
private:
	Image* display;
	unsigned short currMap; //the hlm we are standing in

	MapRepository* mapRepo;

	int minX, maxX, minY, maxY;
	
	void updateMapInDisplay(MapRoom* map);
	void discoverMap(MapRoom* map);
	void visitMap(MapRoom* map, int row, int col);

public:
	AutoMap(WINDOW* win, int height, int width);

	void setMapRepo(MapRepository* mapRepo) { this->mapRepo = mapRepo; }
	void setCurrMap(unsigned short mapId);
	
	Image* getDisplay() { return display; };
	
	void discover(unsigned short mapId);
	void visit(unsigned short mapId, int row, int col);
	void updateDisplay();
	void draw(); //override
};
