#pragma once
#include <list>
#include "MapRoom.h"


std::string getFloorLabel(int floorIndexIn);

//consider renaming this class to Map
class MegaMap : public Thing //, public Controllable
{
private:
	/*A high level map where each tile is a block of a regular map defined by the unitsHigh unitsWide parameters.
	Every tile in the image will contain the map id to be loaded. Image's technically hold chtype, but this is really
	a typedef of unsigned int.
	*/
	std::vector<Image> mapRoomLayout;
	std::vector<Image> autoMap;

	int unitRows = 0;
	int unitCols = 0;
	
	//the height/width of a unit map. All maps loaded should have dimensions that are a multiple of the unit map size
	int unitHeight = 1;
	int unitWidth = 1;

	//cursor being tracked (real map coordinates)
	int* curY = nullptr;
	int* curX = nullptr;

	/*An offset to identify which layer is groundLevel*/
	int groundFloorOffset = 0;

	/*The current layer being occupied by a player, or being displayed.
	By itself the floorIndex identifies the layer in the mapRoomLayout, but with the groundFloorOffset
	we can determine the true floor level (...-2, -1, 0, 1, 2, ...) instead of (0, 1, 2, 3, 4, 5, ...)*/
	int layerIndex = 0;

	/*The floor label is different again from the floor index
	(...-2 = B2, -1 = B1, 0 = 1F, 1 = 2F, 2 = 3F, ...)*/
	std::string floorLabel;

	Pos findMapRoomStart();
public:
	MegaMap(); //set unit height and width, then cursor

	/*the cursor unit level coordinates
	Uy = y / unitHeight;
	Ux = x / unitWidth;
	*/
	Pos getUnitPos();

	//void setDimensions(int height, int width);
	void setDimensions(int height, int width, int depth = 1);

	/*Set the image for one level. True if successfully set. Does not do inserts.
	Dimensions of image must match those set in unit rows and cols*/
	bool setLayerImage(int index, Image& image);

	//getters/setters
	void setUnitHeight(int unitHeightIn) { unitHeight = unitHeightIn; }
	int getUnitHeight() { return unitHeight; }
	void setUnitWidth(int unitWidthIn) { unitWidth = unitWidthIn; }
	int getUnitWidth() { return unitWidth; }
	int getDepth() { return mapRoomLayout.size(); }
	int getUnitRows() { return unitRows; }
	int getUnitCols() { return unitCols; }

	void setCursor(int* y, int* x) { curY = y; curX = x; }


	//room 
	/*the coordinates of the current map room*/
	Pos getMapRoomPos();

	/*Retrieve the id of the current map room the cursor is in*/
	int getCurrMapRoomId();

	/*Get the real height of the map.*/
	int getRealHeight() { return unitRows * unitHeight; }

	/*Get the real width of the map.*/
	int getRealWidth() { return unitCols * unitWidth; }

	std::vector<Image>& getMapRoomLayout() { return mapRoomLayout; }


	//floor stuff

	/*Get the layer index converted to label*/
	std::string& getFloorLabel();
	
	/*Get the int index value of the floor e.g 0 = 1F, 1 = 2F, -1 = B1, -2 = B2 */
	int getFloorIndex();

	/*Set floor using string id B2, B1, 1F, 2F, etc...*/
	bool setFloor(const std::string& floor);
	
	/*Set floor using index of floor 0 = 1F, 1 = 2F, -1 = B1, -2 = B2*/
	bool setFloorIndex(int floorIn);
	bool changeLayer(int amount);

	void setGroundFloor(int groundLevelIn) { groundFloorOffset = groundLevelIn; }
	int getGroundFloor() { return groundFloorOffset; }



	//update automap based on cursor
	void visitArea();
	std::vector<Image>& getAutoMap() { return autoMap; }
	
	//megamap is only drawn when displayed in automap mode
	//void draw();

	//void setWindow(WINDOW* win); //overridden

	//bool isDiscovered(Pos unitPos);
	//void fill(Image& srcImg, Image& destImg, int sourceRow, int sourceCol, chtype fillTile);
};