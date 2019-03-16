#pragma once
//#include "MapRepository.h"
#include <vector>
#include <list>
#include "MegaMap.h"

enum class MapBlockStatus
{
	UNKNOWN, //not drawn
	KNOWN, //drawn in dark blue
	VISITED //drawn in cyan
};

//#define UM_KNOWN 0x01 //this should maybe be part of the hilevel map structure
//#define UM_VISITED 0x02

/*A control used for viewing a mega map.
Will display tiles differently depending on whether they are known or visited.*/
class AutoMap : public Controllable
{
private:
	Image* display = nullptr; //display region for the current map
	//unsigned short currMap; 

	//MapRepository* mapRepo;
	MegaMap* currMap; //pointer to the current map

	int minX, maxX, minY, maxY;
	
	void updateMapInDisplay(MapRoom* map);
	void discoverMap(MapRoom* map);
	void visitMap(MapRoom* map, int row, int col);

public:
	AutoMap() {}
	AutoMap(WINDOW* win, int height, int width);

	//void setMapRepo(MapRepository* mapRepo) { this->mapRepo = mapRepo; }
	void setCurrMap(MegaMap* mapIn);
	
	Image* getDisplay() { return display; };
	
	void discover(unsigned short mapId);
	void visit(unsigned short mapId, int row, int col);
	void updateDisplay();
	void draw(); //override
};
