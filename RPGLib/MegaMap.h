#pragma once
#include "Controllable.h"
#include <list>
#include "MapMetadata.h"
#include "MapRoom.h"

//rename this class to Map
class MegaMap
{
private:
	/*A high level map where each tile is a block of a regular map defined by the unitsHigh unitsWide parameters.
	Every tile in the image will contain the map id to be loaded. Image's technically hold chtype, but this is really
	a typedef of unsigned int.
	*/
	Image mapRoomLayout;
	
	//the height/width of a unit map. All maps loaded should have dimensions that are a multiple of the unit map size
	int unitHeight;
	int unitWidth;

	//cursor being tracked (real map coordinates)
	int* curY;
	int* curX;
	
public:
	MegaMap() {}; //set unit height and width, then cursor

	/*the coordinates of the current map room*/
	Pos getMapRoomPos();

	/*the cursor unit level coordinates
	Uy = y / unitHeight;
	Ux = x / unitWidth;
	*/
	Pos getUnitPos();

	/*The full height of the map.
	unit rows * unitHeight   */
	int getRealHeight();

	/*The full width of the map.
	unit cols * unitWidth   */
	int getRealWidth();

	/*Retrieve the id of the current map room the cursor is in*/
	int getCurrMapRoomId();

	//getters/setters
	void setUnitHeight(int unitHeightIn) { unitHeight = unitHeightIn; }
	int getUnitHeight() { return unitHeight; }
	void setUnitWidth(int unitWidthIn) { unitWidth = unitWidthIn; }
	int getUnitWidth() { return unitWidth; }

	void setCursor(int* y, int* x) { curY = y; curX = x; }
	void setMapRoomLayout(Image& mapDataIn) { mapRoomLayout = mapDataIn; }
	Image& getMapRoomLayout() { return mapRoomLayout; }
};