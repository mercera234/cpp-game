#include "Image.h"
#include <fstream>
#include "TUI.h"

Image::Image(int rows, int cols, WINDOW* win)
{
	Controllable::setDimensions(rows, cols);
	tileMap = new TwoDStorage<chtype>(totalRows, totalCols);

	reset();

	init(win);
}

void Image::init(WINDOW* win)
{
	setWindow(win);
	bordered = true;
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
	chtype c = ' ';
	if (mapX >= 0 && mapY >= 0 && mapX < totalCols && mapY < totalRows) //in the grid
	{
		c = tileMap->getDatum(mapY, mapX);
	}
	else if(bordered)//if negative or beyond the canvas then we are outside the array so draw a box like boundary around the map
	{
		c = getOutOfBoundsTile(mapY, mapX);
	}

	mvwaddch(win, row, col, c);
}


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


int Image::save(ofstream* saveFile)
{
	if (saveFile->is_open() == false)
		return false;

	std::streampos startPos = saveFile->tellp();

	saveFile->write((char*)&totalRows, sizeof(short));
	saveFile->write((char*)&totalCols, sizeof(short));
	
	//TODO class is broken now
	chtype c;
	for (int i = 0; i < totalTiles; i++)
	{
		c = tileMap->getDatum(i);
		saveFile->write((char*)&c, sizeof(chtype));
	}
	//saveFile->write((char*)tileMap->getData(), sizeof(chtype) * totalTiles);

	std::streampos endPos = saveFile->tellp();

	return endPos - startPos;
}


int Image::load(ifstream* loadFile)
{
	if (loadFile->is_open() == false)
		return false;

	std::streampos startPos = loadFile->tellg();

	loadFile->read((char*)&totalRows, sizeof(short));
	loadFile->read((char*)&totalCols, sizeof(short));
	Controllable::setDimensions(totalRows, totalCols); //this sets the variables twice, but that's ok

	if (tileMap == NULL || totalTiles != tileMap->getSize()) //if image is new, or a resize is occurring
	{
		if(tileMap != NULL)
			tileMap->~TwoDStorage(); //if display was already setup then delete the data
			
		tileMap = new TwoDStorage<chtype>(totalRows, totalCols);
	}

	
	chtype c;
	for (int i = 0; i < totalTiles; i++)
	{
		loadFile->read((char*)&c, sizeof(chtype));
		tileMap->setDatum(i, c);
	}

	//loadFile->read((char*)tileMap->getData(), sizeof(chtype) * totalTiles);

	std::streampos endPos = loadFile->tellg();

	return endPos - startPos;
}

/*
//so far works only for resizing the amount of rows. If number of cols changes then the data cannot be copied exactly

*/
void Image::setDimensions(int rows, int cols)
{
	int prevTotalTiles = totalTiles;

	Controllable::setDimensions(rows, cols);

	if (tileMap == NULL) //don't resize tileMap if not initialized
		return;

	TwoDStorage<chtype>* oldTileMap = tileMap;
	TwoDStorage<chtype>* newTileMap = new TwoDStorage<chtype>(rows, cols);

	//for now we will resize to the preferred size, but later on we will only do dynamic resizing to particular sizes
	for (int i = 0; i < totalTiles; i++)
	{
		newTileMap->fill(' ');
	}

	newTileMap->copyFrom(*oldTileMap);
	tileMap = newTileMap;

	delete oldTileMap;
}



Image::~Image()
{
	delete tileMap;
}


Image* Image::getFullScreenShot() //static
{
	return getScreenShot(0, 0, screenHeight, screenWidth);
}

Image* Image::getScreenShot(unsigned int y, unsigned int x, unsigned int height, unsigned int width) //static
{
	//get actual screen dimensions for error checking
	if (x > screenWidth || y > screenHeight) //nothing to return if coordinates are outside of screen
		return NULL;

	//if width/height was set too long, then take a snapshot of what is available
	if (x + width > screenWidth)
		width = screenWidth - x;

	if (y + height > screenHeight)
		height = screenHeight - y;


	//create new snapshot
	Image* snapShot = new Image(height, width, stdscr);

	TwoDStorage<chtype>* tileMap = snapShot->getTileMap();

	for (int i = 0; i < snapShot->getTotalTiles(); i++)
	{
		int row = i / width;
		int col = i % width;
		chtype c = mvwinch(stdscr, row + y, col + x);
		tileMap->setDatum(row, col, c);
	}

	return snapShot;
}