#pragma once
#include "Controllable.h"
#include <list>
#include "MapRoom.h"

//consider renaming this class to Map
class MegaMap : public Thing
{
private:
	/*A high level map where each tile is a block of a regular map defined by the unitsHigh unitsWide parameters.
	Every tile in the image will contain the map id to be loaded. Image's technically hold chtype, but this is really
	a typedef of unsigned int.
	*/
	//Image mapRoomLayout;
	std::vector<Image> mapRoomLayout;

	/*An offset to identify which Image is groundLevel*/
	int groundFloorOffset = 0;

	
	int unitRows = 0;
	int unitCols = 0;
	
	//the height/width of a unit map. All maps loaded should have dimensions that are a multiple of the unit map size
	int unitHeight;
	int unitWidth;

	//cursor being tracked (real map coordinates)
	int* curY;
	int* curX;

	/*The current layer being occupied by a player, or being displayed.
	The index representing the players point of view of what floor they are on*/
	int floorIndex = 0;
	
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

	void setDimensions(int height, int width);
	void setDimensions(int height, int width, int depth);

	/*Set the image for one level. True if successfully set. Does not do inserts.
	Dimensions of image must match those set in unit rows and cols*/
	bool setLayerImage(int floor, Image& image);

	//getters/setters
	void setUnitHeight(int unitHeightIn) { unitHeight = unitHeightIn; }
	int getUnitHeight() { return unitHeight; }
	void setUnitWidth(int unitWidthIn) { unitWidth = unitWidthIn; }
	int getUnitWidth() { return unitWidth; }
	int getDepth() { return mapRoomLayout.size(); }

	int getUnitRows() { return unitRows; }
	int getUnitCols() { return unitCols; }

	void setCursor(int* y, int* x) { curY = y; curX = x; }

	void changeLayer(int amount) { floorIndex += amount; }

	int getFloorIndex();

	std::string getFloorLabel();

	bool setFloor(int floorIn);

	void setGroundFloor(int groundLevelIn) { groundFloorOffset = groundLevelIn; }
	int getGroundLevel() { return groundFloorOffset; }
	int getRealIndexFromFloor(int floor);
	int getFloorFromIndex(int index);

//	void setMapRoomLayout(Image& mapDataIn) { mapRoomLayout = mapDataIn; }
	//Image& getMapRoomLayout() { return mapRoomLayout; }

	std::vector<Image>& getMapRoomLayout() { return mapRoomLayout; }

};