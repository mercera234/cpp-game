#include "Map.h"
#include <fstream>

Map::Map(string name, int rows, int cols, WINDOW* win)
{
	id = 0;
	this->name = name;
	this->rows = rows;
	this->cols = cols;
	totalTiles = rows * cols;
	displayLayer = new chtype[totalTiles];
	effectsLayer = new short[totalTiles];

	reset();

	this->win = win;
	getmaxyx(win, visibleRows, visibleCols);

	ulY = 0;
	ulX = 0;
	filterMask = 0xffffffff;
}

void Map::reset()
{
	for (int row = 0; row < rows; row++)
	{
		for (int col = 0; col < cols; col++)
		{
		/*	chtype c = (chtype)(row % NULL_MARKER_SPACING == 0 && 
							    col % NULL_MARKER_SPACING == 0) ? '!' : ' ';*/
			int element = row * cols + col;
			displayLayer[element] = ' '; //fill display with null template

			effectsLayer[element] = 0; //zero out effects layer
		}
	}
}

void Map::setPosition(int y, int x)
{
	ulY = y;
	ulX = x;
}

void Map::shiftVert(int magnitude)
{
	ulY += magnitude;
}

void Map::shiftHor(int magnitude)
{
	ulX += magnitude;
}

void Map::setDisplayChar(int y, int x, chtype c)
{
	int element = y * cols + x;
	displayLayer[element] = c;
}

chtype Map::getDisplayChar(int y, int x)
{
	int element = y * cols + x;
	return displayLayer[element];
}


void Map::draw()
{
	//get position of upper left corner of map
	/*int ulY = y - centerY;
	int ulX = x - centerX;*/

	/*
	row and col are the position within the viewport
	mapY and mapX are where we are positioned in the map array
	*/

	for (int row = 0, mapY = ulY; row <= visibleRows; row++, mapY++)
	{
		for (int col = 0, mapX = ulX; col <= visibleCols; col++, mapX++)
		{
			drawTileChar(row, col, mapY, mapX);
		}
	}
	wnoutrefresh(win);
}


void Map::drawTileChar(int row, int col, int mapY, int mapX)
{
	chtype c;
	if (mapX >= 0 && mapY >= 0 && mapX < cols && mapY < rows) //in the grid
	{
		int element = mapY * cols + mapX;
		c = displayLayer[element]; //pull chtype from storage
	}
	else //if negative or beyond the canvas then we are outside the array so draw a box like boundary around the map
	{
		c = getOutOfBoundsTile(mapY, mapX);
	}

	mvwaddch(win, row, col, c & filterMask);
}



chtype Map::getOutOfBoundsTile(int mapY, int mapX)
{
	chtype borderChar = ' ';
	if (mapY == -1) //top of boundary
	{
		if (mapX == -1) //upper left boundary
		{
			borderChar = ACS_ULCORNER;
		}
		else if (mapX == cols) //upper right boundary
		{
			borderChar = ACS_URCORNER;
		}
		else if (mapX > -1 && mapX < cols)//a top piece of boundary between the upper corners
		{
			borderChar = ACS_HLINE;
		}
	}
	else if (mapY == rows) //bottom of boundary
	{
		if (mapX == -1) //lower left boundary
		{
			borderChar = ACS_LLCORNER;
		}
		else if (mapX == cols) //lower right boundary
		{
			borderChar = ACS_LRCORNER;
		}
		else if (mapX > -1 && mapX < cols)//a bottom piece of boundary between the lower corners
		{
			borderChar = ACS_HLINE;
		}
	}
	else if ((mapY > -1 && mapY < rows) &&
		(mapX == -1 || mapX == cols))//left or right side
	{
		borderChar = ACS_VLINE;
	}
	return borderChar;
}

//!!!stub!!!
bool Map::save()
{
	ofstream gFile;
	gFile.open(name, ios::trunc | ios::binary);
	if (gFile.is_open() == false)
		return false;

	gFile.write((char*) &id, sizeof(short));
	gFile.write((char*)&rows, sizeof(short));
	gFile.write((char*)&cols, sizeof(short));
	
	gFile.write((char*)displayLayer, sizeof(chtype) * totalTiles);
	gFile.write((char*)effectsLayer, sizeof(short) * totalTiles);

	gFile.close();
}

bool Map::load(string fileName)
{
	ifstream gFile;
	gFile.open(fileName, ios::binary);
	if (gFile.is_open() == false)
		return false;

	gFile.read((char*)&id, sizeof(short));
	gFile.read((char*)&rows, sizeof(short));
	gFile.read((char*)&cols, sizeof(short));
	totalTiles = rows * cols;

	//do arrays need to be initialized first?
	gFile.read((char*)displayLayer, sizeof(chtype) * totalTiles);
	gFile.read((char*)effectsLayer, sizeof(short) * totalTiles);


	gFile.close();
}

Map::~Map()
{
	delwin(win);
	delete displayLayer;
	delete effectsLayer;
}