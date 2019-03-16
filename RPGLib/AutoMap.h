#pragma once
//#include "MapRepository.h"
#include <vector>
#include <list>
#include "MegaMap.h"

//enum class MapBlockStatus
//{
//	UNKNOWN, //not drawn
//	KNOWN, //drawn in dark blue
//	VISITED //drawn in cyan
//};

//#define UM_KNOWN 0x01 //this should maybe be part of the hilevel map structure
//#define UM_VISITED 0x02

/*A control used for viewing a mega map.
Will display tiles differently depending on whether they are known or visited.*/
class AutoMap : public Controllable, public InputProcessor
{
private:
	MegaMap* currMap = nullptr; //pointer to the current map
	int viewLayer = 0; //the current layer in map being viewed

	//Image* display = nullptr; //display region for the current map
	//unsigned short currMap; 

	//MapRepository* mapRepo;
	//int minX, maxX, minY, maxY;
	
	//void updateMapInDisplay(MapRoom* map);
	//void discoverMap(MapRoom* map);
	//void visitMap(MapRoom* map, int row, int col);

	void alterViewLayer(int amount);
public:
	AutoMap() {}
	
	void setCurrMap(MegaMap* mapIn);
	void setWindow(WINDOW* win); //override
	void reset();

	void setPosition(int y, int x);

	//AutoMap(WINDOW* win, int height, int width);
	MegaMap* getCurrMap() { return currMap; }

	void processInput();
	//void setMapRepo(MapRepository* mapRepo) { this->mapRepo = mapRepo; }
	//Image* getDisplay() { return display; };
	
	//void discover(unsigned short mapId);
	//void visit(unsigned short mapId, int row, int col);
	//void updateDisplay();
	void draw(); //override
};
