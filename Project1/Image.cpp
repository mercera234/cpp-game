#include "Image.h"
#include <fstream>

Image::Image(int rows, int cols, WINDOW* win)
{
	this->totalRows = rows;
	this->totalCols = cols;
	totalTiles = totalRows * totalCols;
	//displayLayer = new chtype[totalTiles];

	tileMap = new _2DStorage<chtype>(totalRows, totalCols);

	reset();

	init(win);
}

void Image::init(WINDOW* win)
{
	setWindow(win);
	imgY = 0;
	imgX = 0;
}

Image::Image(WINDOW* win)
{
	init(win);
}

void Image::reset()
{
	for (int row = 0; row < totalRows; row++)
	{
		for (int col = 0; col < totalCols; col++)
		{
		/*	chtype c = (chtype)(row % NULL_MARKER_SPACING == 0 && 
							    col % NULL_MARKER_SPACING == 0) ? '!' : ' ';*/
			//int element = row * totalCols + col;
			//displayLayer[element] = ' '; //fill display with null template

			tileMap->setDatum(row, col, ' ');
		}
	}
}


void Image::draw()
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


void Image::drawTileChar(int row, int col, int mapY, int mapX)
{
	chtype c;
	if (mapX >= 0 && mapY >= 0 && mapX < totalCols && mapY < totalRows) //in the grid
	{
		//int element = mapY * cols + mapX;
		//c = displayLayer[element]; //pull chtype from storage
		c = tileMap->getDatum(mapY, mapX);
	}
	else //if negative or beyond the canvas then we are outside the array so draw a box like boundary around the map
	{
		c = getOutOfBoundsTile(mapY, mapX);
	}

	mvwaddch(win, row, col, c);
}

//void Image::drawTileChar(int row, int col, int mapY, int mapX)
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



chtype Image::getOutOfBoundsTile(int mapY, int mapX)
{
	chtype borderChar = ' ';
	if (mapY == -1) //top of boundary
	{
		if (mapX == -1) //upper left boundary
		{
			borderChar = ACS_ULCORNER;
		}
		else if (mapX == totalCols) //upper right boundary
		{
			borderChar = ACS_URCORNER;
		}
		else if (mapX > -1 && mapX < totalCols)//a top piece of boundary between the upper corners
		{
			borderChar = ACS_HLINE;
		}
	}
	else if (mapY == totalRows) //bottom of boundary
	{
		if (mapX == -1) //lower left boundary
		{
			borderChar = ACS_LLCORNER;
		}
		else if (mapX == totalCols) //lower right boundary
		{
			borderChar = ACS_LRCORNER;
		}
		else if (mapX > -1 && mapX < totalCols)//a bottom piece of boundary between the lower corners
		{
			borderChar = ACS_HLINE;
		}
	}
	else if ((mapY > -1 && mapY < totalRows) &&
		(mapX == -1 || mapX == totalCols))//left or right side
	{
		borderChar = ACS_VLINE;
	}
	return borderChar;
}


bool Image::save(ofstream saveFile)
{
	/*ofstream saveFile;
	saveFile.open(fileName, ios::trunc | ios::binary);
	*/
	
	if (saveFile.is_open() == false)
		return false;

//	saveFile.write((char*) &id, sizeof(short));
	saveFile.write((char*)&totalRows, sizeof(short));
	saveFile.write((char*)&totalCols, sizeof(short));
	
	//TODO need to save tileMap here
	//saveFile.write((char*)displayLayer, sizeof(chtype) * totalTiles);

	//saveFile.close();
}


bool Image::load(ifstream* loadFile)
{
//	gFile.open(fileName, ios::binary);
	if (loadFile->is_open() == false)
		return false;

//	loadFile->read((char*)&id, sizeof(short));
	loadFile->read((char*)&totalRows, sizeof(short));
	loadFile->read((char*)&totalCols, sizeof(short));
	totalTiles = totalRows * totalCols;

	if (tileMap == NULL || totalTiles != tileMap->getSize())
	{
		if(tileMap != NULL)
			tileMap->~_2DStorage(); //if display was already setup then delete the data
			
		tileMap = new _2DStorage<chtype>(totalRows, totalCols);

	//	delete displayLayer;
	//	displayLayer = new chtype[totalTiles];
	}

//	int displayPos = loadFile->tellg();
//	loadFile->read((char*)displayLayer, sizeof(chtype) * totalTiles);

//	loadFile->seekg(displayPos);
	loadFile->read((char*)tileMap->getData(), sizeof(chtype) * totalTiles);
	
	//loadFile.close();
}

/*
Works, 
and resizing to fixed sizes instead of exactly what we specify
	The map object will need to modify to account for this as well
*/
void Image::resize(int rows, int cols)
{
	//this->totalRows = rows;
	//this->totalCols = cols;

	//int prevTotalTiles = totalTiles;
	//totalTiles = rows * cols;

	////for now we will resize to the preferred size, but later on we will only do dynamic resizing to particular sizes
	//chtype* newDisplayLayer = new chtype[totalTiles];
	//short* newEffectsLayer = new short[totalTiles];

	//for (int i = 0; i < totalTiles; i++)
	//{
	//	newDisplayLayer[i] = ' ';
	//	newEffectsLayer[i] = 0; //zero out effects layer
	//}

	//chtype* oldArray = displayLayer;

	//chtype* dlOut = newDisplayLayer;
	//chtype* dlIn = displayLayer;
	//
	////use the smaller of the two sizes
	//int tilesToCopy = prevTotalTiles < totalTiles ? prevTotalTiles : totalTiles;

	//for (int i = 0; i < tilesToCopy; i++)
	//{
	//	*dlOut++ = *dlIn++;
	//}

	//displayLayer = newDisplayLayer;

	//delete oldArray;
}



Image::~Image()
{
	delwin(win);
	//delete displayLayer;
	delete tileMap;
}