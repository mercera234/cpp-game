#include "Map.h"
#include <fstream>

Map::Map(string name, int rows, int cols, WINDOW* win)
{
	id = 0;
	this->name = name;
	this->totalRows = rows;
	this->totalCols = cols;
	totalTiles = totalRows * totalCols;
//	displayLayer = new chtype[totalTiles];
	effectsLayer = new short[totalTiles];

//	dLayer = new _2DStorage<int>(totalRows, totalCols);
	eLayer = new _2DStorage<int>(totalRows, totalCols);
	display = new Image(rows, cols, win); //use same window as map

	reset();

	setWindow(win);
	/*this->win = win;
	
	getmaxyx(win, visibleRows, visibleCols);

	ulY = 0;
	ulX = 0;*/
	mapY = 0; 
	mapX = 0;
	
}

_2DStorage<int>* Map::getLayer(int layer)
{
	_2DStorage<int>* strg = NULL;
	switch (layer)
	{
//	case LAYER_DISPLAY: strg = dLayer; break;
	case LAYER_EFFECT: strg = eLayer; break;
	}
	return strg;
}

void Map::reset()
{
	for (int row = 0; row < totalRows; row++)
	{
		for (int col = 0; col < totalCols; col++)
		{
		/*	chtype c = (chtype)(row % NULL_MARKER_SPACING == 0 && 
							    col % NULL_MARKER_SPACING == 0) ? '!' : ' ';*/
			int element = row * totalCols + col;
	//		displayLayer[element] = ' '; //fill display with null template

	//		dLayer->setDatum(row, col, ' ');

			eLayer->setDatum(row, col, 0);

			effectsLayer[element] = 0; //zero out effects layer
		}
	}

	display->reset();
}


//void Map::setDisplayChar(int y, int x, chtype c)
//{
//	int element = y * totalCols + x;
//	displayLayer[element] = c;
//}
//
//chtype Map::getDisplayChar(int y, int x)
//{
//	int element = y * totalCols + x;
//	return displayLayer[element];
//}

void Map::setEffect(int y, int x, short effect)
{
	int element = y * totalCols + x;
	effectsLayer[element] |= effect;
}

void Map::removeEffect(int y, int x, short effect)
{
	int element = y * totalCols + x;
	effectsLayer[element] &= ~effect;
}

short Map::getEffects(int y, int x)
{
	int element = y * totalCols + x;
	return effectsLayer[element];
}

void Map::draw()
{
	//get position of upper left corner of map
	/*
	row and col are the position within the viewport
	mapY and mapX are where we are positioned in the map array
	*/

	display->draw();
	/*for (int row = 0, mapY = ulY; row <= visibleRows; row++, mapY++)
	{
		for (int col = 0, mapX = ulX; col <= visibleCols; col++, mapX++)
		{
			drawTileChar(row, col, mapY, mapX);
		}
	}
	wnoutrefresh(win);*/
}


//void Map::drawTileChar(int row, int col, int mapY, int mapX)
//{
//	chtype c;
//	if (mapX >= 0 && mapY >= 0 && mapX < totalCols && mapY < totalRows) //in the grid
//	{
//		//int element = mapY * cols + mapX;
//		//c = displayLayer[element]; //pull chtype from storage
//		c = dLayer->getDatum(mapY, mapX);
//	}
//	else //if negative or beyond the canvas then we are outside the array so draw a box like boundary around the map
//	{
//		c = getOutOfBoundsTile(mapY, mapX);
//	}
//
//	mvwaddch(win, row, col, c & filterMask);
//}

//void Map::drawTileChar(int row, int col, int mapY, int mapX)
//{
//	chtype c;
//	if (mapX >= 0 && mapY >= 0 && mapX < totalCols && mapY < totalRows) //in the grid
//	{
//		int element = mapY * totalCols + mapX;
//		c = displayLayer[element]; //pull chtype from storage
//	}
//	else //if negative or beyond the canvas then we are outside the array so draw a box like boundary around the map
//	{
//		c = getOutOfBoundsTile(mapY, mapX);
//	}
//
//	mvwaddch(win, row, col, c & filterMask);
//}



//chtype Map::getOutOfBoundsTile(int mapY, int mapX)
//{
//	chtype borderChar = ' ';
//	if (mapY == -1) //top of boundary
//	{
//		if (mapX == -1) //upper left boundary
//		{
//			borderChar = ACS_ULCORNER;
//		}
//		else if (mapX == totalCols) //upper right boundary
//		{
//			borderChar = ACS_URCORNER;
//		}
//		else if (mapX > -1 && mapX < totalCols)//a top piece of boundary between the upper corners
//		{
//			borderChar = ACS_HLINE;
//		}
//	}
//	else if (mapY == totalRows) //bottom of boundary
//	{
//		if (mapX == -1) //lower left boundary
//		{
//			borderChar = ACS_LLCORNER;
//		}
//		else if (mapX == totalCols) //lower right boundary
//		{
//			borderChar = ACS_LRCORNER;
//		}
//		else if (mapX > -1 && mapX < totalCols)//a bottom piece of boundary between the lower corners
//		{
//			borderChar = ACS_HLINE;
//		}
//	}
//	else if ((mapY > -1 && mapY < totalRows) &&
//		(mapX == -1 || mapX == totalCols))//left or right side
//	{
//		borderChar = ACS_VLINE;
//	}
//	return borderChar;
//}


bool Map::save(string fileName)
{
	ofstream gFile;
	gFile.open(fileName, ios::trunc | ios::binary);
	if (gFile.is_open() == false)
		return false;

	gFile.write((char*) &id, sizeof(short));
	gFile.write((char*)&totalRows, sizeof(short));
	gFile.write((char*)&totalCols, sizeof(short));
	
	//gFile.write((char*)displayLayer, sizeof(chtype) * totalTiles);
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

	display->load(&gFile);
	/*gFile.read((char*)&totalRows, sizeof(short));
	gFile.read((char*)&totalCols, sizeof(short));
	totalTiles = totalRows * totalCols;
*/
	//do arrays need to be initialized first?
//	gFile.read((char*)displayLayer, sizeof(chtype) * totalTiles);
	gFile.read((char*)effectsLayer, sizeof(short) * totalTiles);


	gFile.close();
}


bool Map::load2(string fileName)
{
	ifstream gFile;
	gFile.open(fileName, ios::binary);
	if (gFile.is_open() == false)
		return false;

	gFile.read((char*)&id, sizeof(short));
	gFile.read((char*)&totalRows, sizeof(short));
	gFile.read((char*)&totalCols, sizeof(short));
	totalTiles = totalRows * totalCols;

	//do arrays need to be initialized first?



	//gFile.read((char*)displayLayer, sizeof(chtype) * totalTiles);
//	gFile.read((char*)dLayer->getData(), sizeof(chtype) * totalTiles);
	gFile.read((char*)effectsLayer, sizeof(short) * totalTiles);

	//since effects layer has changed lets copy the effects into the e-layer
	for (int i = 0; i < totalTiles; i++)
	{
		eLayer->setDatum(i, (int)effectsLayer[i]);
	}

	//gFile.read((char*)eLayer->getData(), sizeof(short) * totalTiles);
	

	


	gFile.close();
}

/*
Works, 
and resizing to fixed sizes instead of exactly what we specify
	The map object will need to modify to account for this as well
*/
void Map::resize(int rows, int cols)
{
	this->totalRows = rows;
	this->totalCols = cols;

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

//	chtype* oldArray = displayLayer;
	short* oldEffects = effectsLayer;

	chtype* dlOut = newDisplayLayer;
//	chtype* dlIn = displayLayer;
	short* elOut = newEffectsLayer;
	short* elIn = effectsLayer;

	//use the smaller of the two sizes
	int tilesToCopy = prevTotalTiles < totalTiles ? prevTotalTiles : totalTiles;

	for (int i = 0; i < tilesToCopy; i++)
	{
//		*dlOut++ = *dlIn++;
		*elOut++ = *elIn++;
	}

//	displayLayer = newDisplayLayer;
	effectsLayer = newEffectsLayer;

//	delete oldArray;
	delete oldEffects;
}



Map::~Map()
{
	delwin(win);
//	delete displayLayer;
	delete effectsLayer;
	
//	delete dLayer;
	delete eLayer;
	delete display;
}