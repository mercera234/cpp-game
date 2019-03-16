#include "MegaMap.h"
#include "TUI.h"
#include <sstream>

const chtype unknown = ' ';
const chtype known = ' ' | setBkgdColor(COLOR_BLUE);
const chtype visited = ' ' | setBkgdColor(COLOR_CYAN);


//a null cursor position
Pos nullPos(0, 0);

MegaMap::MegaMap()
{
	//so cursor is always attached to something
	curY = &nullPos.y;
	curX = &nullPos.x;
}

Pos MegaMap::getMapRoomPos()
{
	Pos imc; //internal map coordinates

	if (layerIndex < 0 || layerIndex >= (int)mapRoomLayout.size())
	{
		imc.y = -1;
		imc.x = -1;
		return imc;
	}

	Pos startPos = findMapRoomStart();
	
	imc.y = *curY - (startPos.y) * unitHeight;
	imc.x = *curX - (startPos.x) * unitWidth;

	return imc;
}

Pos MegaMap::findMapRoomStart()
{
	Pos unitPos = getUnitPos();
	Image& mapLayer = mapRoomLayout[layerIndex];

	int mapId = mapLayer.getTile(unitPos.y, unitPos.x);

	if (mapId < 0) //null map loaded(current map is considered to be just the one screen)
	{
		return unitPos;
	}

	//find start point of map
	Pos walkPos(unitPos.y, unitPos.x);
	
	//walk to top of map room
	while (walkPos.y >= 0 && mapLayer.getTile(walkPos.y, walkPos.x) == mapId)
	{
		walkPos.y--;
	}
	walkPos.y++; //correct back 1
	
	//walk to left of map room
	while (walkPos.x >= 0 && mapLayer.getTile(walkPos.y, walkPos.x) == mapId)
	{
		walkPos.x--;
	}
	walkPos.x++; //correct back 1
	
	return walkPos; //pos is in upper left corner of current map
}


Pos MegaMap::getUnitPos()
{
	Pos unitCoords(*curY / unitHeight, *curX / unitWidth);
	return unitCoords;
}


int MegaMap::getCurrMapRoomId()
{
	Pos map = getUnitPos();

	return mapRoomLayout[layerIndex].getTile(map.y, map.x);
}

void MegaMap::setDimensions(int height, int width, int depth)
{
	unitRows = height;
	unitCols = width;
	mapRoomLayout.resize(depth); //only holding one row
	autoMap.resize(depth);
}

bool MegaMap::setLayerImage(int index, Image& image)
{
	if (index < 0 || index >= (int)mapRoomLayout.size())
	{
		return false; //cannot insert layer at this level
	}

	if (image.getTotalCols() != unitCols || image.getTotalRows() != unitRows)
	{
		return false;
	}

	mapRoomLayout[index] = image;

	//add layer to automap as well
	Image autoMapImg;
	autoMapImg.setDimensions(image.getTotalRows(), image.getTotalCols());	
	autoMapImg.reset();
	autoMap[index] = autoMapImg;
	return true;
}

int MegaMap::getFloorIndex()
{ 
	return layerIndex - groundFloorOffset; 
}

std::string& MegaMap::getFloorLabel()
{
	/*std::ostringstream oss;
	int floor = layerIndex - groundFloorOffset;
	if (floor >= 0)
	{
		oss << (floor + 1) << 'F';
	}
	else
	{
		oss << 'B' << abs(floor);
	}
	*/
	floorLabel = ::getFloorLabel(layerIndex - groundFloorOffset);
	return floorLabel;
}

std::string getFloorLabel(int floorIndex)
{
	std::ostringstream oss;
	if (floorIndex >= 0)
	{
		oss << (floorIndex + 1) << 'F';
	}
	else
	{
		oss << 'B' << abs(floorIndex);
	}

	return oss.str();
}


bool MegaMap::setFloor(const std::string& floor)
{
	int floorIndex;

	//convert floor back to floorIndex
	if (floor[0] == 'B')
	{
		floorIndex = -(floor[1] - '0');
	}
	else if (floor[1] == 'F')
	{
		floorIndex = floor[0] - '0' - 1;
	}
	else
		return false;

	return setFloorIndex(floorIndex);
}

bool MegaMap::setFloorIndex(int floorIn)
{
	return changeLayer(floorIn + groundFloorOffset - layerIndex);
}

bool MegaMap::changeLayer(int amount)
{
	int tempIndex = layerIndex + amount;
	if (tempIndex < 0 || tempIndex >= (int)mapRoomLayout.size())
		return false;

	layerIndex = tempIndex;
	return true;
}

void MegaMap::visitArea()
{
	Pos unitPos = getUnitPos();

	chtype currUnitMap = autoMap[layerIndex].getTile(unitPos.y, unitPos.x);

	Image& mapLayer = mapRoomLayout[layerIndex];
	Image& autoMapLayer = autoMap[layerIndex];

	if (currUnitMap == unknown) //not visited
	{
		//make current maproom known(locate upper left corner of curr map room and fill with dark blue
		Pos walkPos = findMapRoomStart();
		int mapId = mapLayer.getTile(unitPos.y, unitPos.x);
		
		//fill in automap with known blocks
		Pos upperLeft = walkPos;
		while (upperLeft.y < mapLayer.getTotalRows() && mapLayer.getTile(upperLeft.y, upperLeft.x) == mapId)
		{
			while (walkPos.x < mapLayer.getTotalCols() && mapLayer.getTile(walkPos.y, walkPos.x) == mapId)
			{
				autoMapLayer.setTile(walkPos.y, walkPos.x, known);
				walkPos.x++;
			}
			upperLeft.y++;
			walkPos = upperLeft;
		}

		//visit current unit map
		autoMapLayer.setTile(unitPos.y, unitPos.x, visited);
	}
	else if (currUnitMap == known)
	{
		autoMapLayer.setTile(unitPos.y, unitPos.x, visited);
	}

}


//void MegaMap::draw()
//{
//	//draw only the image being shown
//	Image& autoMapLayer = autoMap[layerIndex];
//	autoMapLayer.draw();
//
//	//draw floor in lower right corner
//	mvwaddstr(win, autoMapLayer.getTotalRows() - 1, autoMapLayer.getTotalCols() - 2, getFloorLabel().c_str());
//}
//
//void MegaMap::setWindow(WINDOW* win)
//{
//	Controllable::setWindow(win);
//
//	std::for_each(autoMap.begin(), autoMap.end(),
//		[win](Image& img) {
//		img.setWindow(win);
//
//	});
//}




//void MegaMap::fill(Image& srcImg, Image& destImg, int sourceRow, int sourceCol, chtype fillTile)
//{
//	std::list<int> fillPoints;
//
//	chtype replaceTile = srcImg.getTile(sourceRow, sourceCol);
//	//chtype fillTile = drawChar | bkgdColor | textColor;
//	//if (drawChar == ' ')
//	//{
//	//	fillTile = drawChar | bkgdColor; //we don't need the text color for blank spaces
//	//}
//
//	int totalCols = srcImg.getTotalCols();
//	int totalRows = srcImg.getTotalRows();
//
//	if (replaceTile != fillTile) //meaning we have chosen the same fill character as what the cursor is currently over
//		fillPoints.push_back(sourceRow * totalCols + sourceCol);
//
//	int editTile;
//	while (fillPoints.empty() == false)
//	{
//		editTile = fillPoints.front(); //consume head of queue
//		fillPoints.pop_front();
//
//		int y = editTile / totalCols;
//		int x = editTile % totalCols;
//
//		destImg.setTile(y, x, fillTile);
//
//		//check adjacent positions to see if they have the replace character
//		int rightTile = x + 1;
//		if (rightTile < totalCols)//make sure it is on fillable map area
//		{
//			if (srcImg.getTile(y, rightTile) == replaceTile)
//			{
//				fillPoints.push_back(y * totalCols + rightTile);
//				destImg.setTile(y, rightTile, fillTile);
//			}
//		}
//
//		int leftTile = x - 1;
//		if (leftTile >= 0)//make sure it is on fillable map area
//		{
//			if (srcImg.getTile(y, leftTile) == replaceTile)
//			{
//				fillPoints.push_back(y * totalCols + leftTile);
//				destImg.setTile(y, leftTile, fillTile);
//			}
//		}
//
//		int downTile = y + 1;
//		if (downTile < totalRows)//make sure it is on fillable map area
//		{
//			if (srcImg.getTile(downTile, x) == replaceTile)
//			{
//				fillPoints.push_back(downTile * totalCols + x);
//				destImg.setTile(downTile, x, fillTile);
//			}
//		}
//
//		int upTile = y - 1;
//		if (upTile >= 0)//make sure it is on fillable map area
//		{
//			if (srcImg.getTile(upTile, x) == replaceTile)
//			{
//				fillPoints.push_back(upTile * totalCols + x);
//				destImg.setTile(upTile, x, fillTile);
//			}
//		}
//	}
//}

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
