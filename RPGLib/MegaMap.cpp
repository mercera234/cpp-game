#include "MegaMap.h"
#include "TUI.h"
#include <sstream>


//Saving this, since we may need it for automap
//void MegaMap::drawMap(MapMetadata* map)
//{
//	char c = map->visualId;
//
//	int mapStartRow = map->hY - ulY; //translate megamap coordinates to window
//	int mapStartCol = map->hX - ulX;
//
//	if (mapStartRow >= visibleRows || mapStartCol >= visibleCols)
//		return; //map is completely off screen, nothing to draw
//
//
//	int mapEndRow = mapStartRow + map->unitsTall;
//	int mapEndCol = mapStartCol + map->unitsWide;
//
//	if (mapEndRow <= 0 || mapEndCol <= 0)
//		return; //map is completely off screen, nothing to draw
//
//	//we've proven that the map is at least partially on screen, now draw the portion
//	int startRow = mapStartRow < 0 ? 0 : mapStartRow; //translate megamap coordinates to window
//	int startCol = mapStartCol < 0 ? 0 : mapStartCol;
//
//	int endRow = mapEndRow > visibleRows ? visibleRows : mapEndRow;
//	int endCol = mapEndCol > visibleCols ? visibleCols : mapEndCol;
//
//	for (int row = startRow; row < endRow; row++)
//	{
//		for (int col = startCol; col < endCol; col++)
//		{
//			mvwaddch(win, row, col, c | bkgd | (COLOR_GRAY << 24));
//		}
//	}
//}

Pos MegaMap::getMapRoomPos()
{
	Pos map = getUnitPos();
	Pos imc;

	if (floorIndex < 0 || floorIndex >= (int)mapRoomLayout.size())
	{
		imc.y = -1;
		imc.x = -1;
		return imc;
	}

	Image& img = mapRoomLayout[floorIndex];

	int tile = img.getTile(map.y, map.x);

	if (tile < 0) //null map loaded
	{
		Pos oob(*curY - map.y * unitHeight , 
			*curX - map.x * unitWidth);
		return oob;
	}

	//find start point of map
	int startRow = map.y;
	for (int row = map.y - 1; row >= 0; row--)
	{
		int newTile = img.getTile(row, map.x);
		if (newTile != tile)
		{
			break;
		}
		else
			startRow = row;
	}
	
	int startCol = map.x;
	for (int col = map.x - 1; col >= 0; col--)
	{
		int newTile = img.getTile(map.y, col);
		if (newTile != tile)
		{
			break;
		}
		else
			startCol = col;
	}

	
	imc.y = *curY - (startRow) * unitHeight;
	imc.x = *curX - (startCol) * unitWidth;

	return imc;
}

Pos MegaMap::getUnitPos()
{
	Pos unitCoords(*curY / unitHeight, *curX / unitWidth);
	return unitCoords;
}


int MegaMap::getRealHeight()
{
	return unitRows * unitHeight;
}

int MegaMap::getRealWidth()
{
	return unitCols * unitWidth;
}

int MegaMap::getCurrMapRoomId()
{
	Pos map = getUnitPos();

	return mapRoomLayout[floorIndex].getTile(map.y, map.x);
}

void MegaMap::setDimensions(int height, int width)
{
	setDimensions(height, width, 1);
}

void MegaMap::setDimensions(int height, int width, int depth)
{
	unitRows = height;
	unitCols = width;
	mapRoomLayout.resize(depth); //only holding one row
}

bool MegaMap::setLayerImage(int floor, Image& image)
{
	//convert level to vector index
	int index = floor + groundFloorOffset;
	
	if (index < 0 || index >= (int)mapRoomLayout.size())
	{
		return false; //cannot insert layer at this level
	}

	if (image.getTotalCols() != unitCols || image.getTotalRows() != unitRows)
	{
		return false;
	}

	mapRoomLayout[index] = image;
	return true;
}

int MegaMap::getFloorIndex()
{ 
	return floorIndex - groundFloorOffset; 
}

std::string& MegaMap::getFloorLabel()
{
	std::ostringstream oss;
	int floor = floorIndex - groundFloorOffset;
	if (floor >= 0)
	{
		oss << (floor + 1) << 'F';
	}
	else
	{
		oss << 'B' << abs(floor);
	}

	floorLabel = oss.str();
	return floorLabel;
}

bool MegaMap::setFloor(int floorIn)
{
	int tempIndex = floorIn + groundFloorOffset;
	if (tempIndex < 0 || tempIndex >= (int)mapRoomLayout.size())
		return false;

	floorIndex = tempIndex;
	return true;
}

int MegaMap::getRealIndexFromFloor(int floor)
{
	return floor + groundFloorOffset;
}

int MegaMap::getFloorFromIndex(int index)
{
	return index - groundFloorOffset;
}







