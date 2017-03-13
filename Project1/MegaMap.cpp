#include "MegaMap.h"
#include "TUI.h"

MegaMap::MegaMap(WINDOW* win)
{
	maps = new list<MapMetadata*>();
	bkgd = (COLOR_BLUE << 28);
	setWindow(win);
}

void MegaMap::addMap(MapMetadata* map)
{
	maps->push_back(map);
}


bool MegaMap::removeMap(int mapId)
{
	bool removed = false;

	list<MapMetadata*>::iterator it;
	for (it = maps->begin(); it != maps->end(); it++)
	{
		MapMetadata* map = *it;
		if (map->mapId == mapId)
		{
			maps->remove(map);
			removed = true;
			break; //stop after finding map with matching id
		}
			
	}
	return removed;
}

void MegaMap::draw()
{
	wclear(win);
	list<MapMetadata*>::iterator it;
	for (it = maps->begin(); it != maps->end(); it++)
	{
		//draw each mapmetadata
		MapMetadata* map = *it;
		drawMap(map);
	}
	wnoutrefresh(win);
}

void MegaMap::drawMap(MapMetadata* map)
{
	char c = map->visualId;

	int mapStartRow = map->hY - ulY; //translate megamap coordinates to window
	int mapStartCol = map->hX - ulX;

	if (mapStartRow >= visibleRows || mapStartCol >= visibleCols)
		return; //map is completely off screen, nothing to draw


	int mapEndRow = mapStartRow + map->unitHeight;
	int mapEndCol = mapStartCol + map->unitWidth;

	if (mapEndRow <= 0 || mapEndCol <= 0)
		return; //map is completely off screen, nothing to draw

	//we've proven that the map is at least partially on screen, now draw the portion
	int startRow = mapStartRow < 0 ? 0 : mapStartRow; //translate megamap coordinates to window
	int startCol = mapStartCol < 0 ? 0 : mapStartCol;

	int endRow = mapEndRow > visibleRows ? visibleRows : mapEndRow;
	int endCol = mapEndCol > visibleCols ? visibleCols : mapEndCol;

	for (int row = startRow; row < endRow; row++)
	{
		for (int col = startCol; col < endCol; col++)
		{
			mvwaddch(win, row, col, c | bkgd | (COLOR_GRAY << 24));
		}
	}
}

