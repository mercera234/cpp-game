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
	getmaxyx(win, totalRows, totalCols);


	ulY = 0;
	ulX = 0;
	mapY = 0; 
	mapX = 0;
	
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

	mapY = -y;
	mapX = -x;
}

void Map::shift(int y, int x)
{
	setPosition(ulY + y, ulX + x);
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

void Map::setEffect(int y, int x, short effect)
{
	int element = y * cols + x;
	effectsLayer[element] |= effect;
}

void Map::removeEffect(int y, int x, short effect)
{
	int element = y * cols + x;
	effectsLayer[element] &= ~effect;
}

short Map::getEffects(int y, int x)
{
	int element = y * cols + x;
	return effectsLayer[element];
}

void Map::draw()
{
	//get position of upper left corner of map
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


bool Map::save(string fileName)
{
	ofstream gFile;
	gFile.open(fileName, ios::trunc | ios::binary);
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

/*
Works, 
and resizing to fixed sizes instead of exactly what we specify
	The map object will need to modify to account for this as well
*/
void Map::resize(int rows, int cols)
{
	this->rows = rows;
	this->cols = cols;

	int prevTotalTiles = totalTiles;
	totalTiles = rows * cols;

	//for now we will resize to the preferred size, but later on we will only do dynamic resizing to particular sizes
	chtype* newDisplayLayer = new chtype[totalTiles];
	short* newEffectsLayer = new short[totalTiles];

	for (int i = 0; i < totalTiles; i++)
	{
		newDisplayLayer[i] = ' ';
		newEffectsLayer[i] = 0; //zero out effects layer
	}

	chtype* oldArray = displayLayer;
	short* oldEffects = effectsLayer;

	chtype* dlOut = newDisplayLayer;
	chtype* dlIn = displayLayer;
	short* elOut = newEffectsLayer;
	short* elIn = effectsLayer;

	//use the smaller of the two sizes
	int tilesToCopy = prevTotalTiles < totalTiles ? prevTotalTiles : totalTiles;

	for (int i = 0; i < tilesToCopy; i++)
	{
		*dlOut++ = *dlIn++;
		*elOut++ = *elIn++;
	}

	displayLayer = newDisplayLayer;
	effectsLayer = newEffectsLayer;

	delete oldArray;
	delete oldEffects;
}



Map::~Map()
{
	delwin(win);
	delete displayLayer;
	delete effectsLayer;
}