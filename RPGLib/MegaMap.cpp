#include "MegaMap.h"
#include "TUI.h"

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
	int tile = mapRoomLayout.getTile(map.y, map.x);

	if (tile == INT_MAX)
	{
		Pos oob(-1, -1);
		return oob;
	}

	//find start point of map
	int startRow = map.y;
	for (int row = map.y - 1; row >= 0; row--)
	{
		int newTile = mapRoomLayout.getTile(row, map.x);
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
		int newTile = mapRoomLayout.getTile(map.y, col);
		if (newTile != tile)
		{
			break;
		}
		else
			startCol = col;
	}

	Pos imc;
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
	return mapRoomLayout.getTotalRows() * unitHeight;
}

int MegaMap::getRealWidth()
{
	return mapRoomLayout.getTotalCols() * unitWidth;
}

int MegaMap::getCurrMapRoomId()
{
	Pos map = getUnitPos();


	return mapRoomLayout.getTile(map.y, map.x);
}














