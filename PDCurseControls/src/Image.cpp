#include <fstream>
#include "Image.h"
#include "TUI.h"

const int NULL_MARKER_SPACING = 8;

Image::Image()
{
	init();
}


Image::Image(WINDOW* win)
{
	init();
	setWindow(win);
}

Image::Image(int rows, int cols, WINDOW* win)
{
	init();
	setWindow(win);

	setDimensions(rows, cols);	
}

void Image::init()
{
	bordered = true;
}


void Image::reset()
{
	for (unsigned int row = 0; row < totalRows; row++)
	{
		for (unsigned int col = 0; col < totalCols; col++)
		{
			tileMap.setDatum(row, col, ' ');
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
	int mapY = ulY;
	for (unsigned short row = 0; row <= visibleRows; row++, mapY++)
	{
		int mapX = ulX;
		for (unsigned short col = 0; col <= visibleCols; col++, mapX++)
		{
			drawTileChar(row, col, mapY, mapX);
		}
	}
	wnoutrefresh(win);
}


void Image::drawTileChar(unsigned short row, unsigned short col, int mapY, int mapX)
{
	chtype c = ' ';
	if (mapX >= 0 && mapY >= 0 && mapX < (int)totalCols && mapY < (int)totalRows) //in the grid
	{
		c = tileMap.getDatum(mapY, mapX);
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
		else if (mapX > -1 && mapX < (int)totalCols)//a top piece of boundary between the upper corners
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
		else if (mapX > -1 && mapX < (int)totalCols)//a bottom piece of boundary between the lower corners
		{
			borderChar = ACS_HLINE;
		}
	}
	else if ((mapY > -1 && mapY < (int)totalRows) &&
		(mapX == -1 || mapX == totalCols))//left or right side
	{
		borderChar = ACS_VLINE;
	}
	return borderChar;
}


int Image::save(std::ofstream& saveFile)
{
	if (saveFile.is_open() == false)
		return false;

	std::streampos startPos = saveFile.tellp();

	saveFile.write((char*)&totalRows, sizeof(short));
	saveFile.write((char*)&totalCols, sizeof(short));
	
	chtype c;
	for (unsigned int i = 0; i < totalTiles; i++)
	{
		c = tileMap.getDatum(i);
		saveFile.write((char*)&c, sizeof(chtype));
	}
	
	std::streampos endPos = saveFile.tellp();

	return (int)(endPos - startPos);
}


int Image::load(std::ifstream& loadFile)
{
	if (loadFile.is_open() == false)
		return false;

	std::streampos startPos = loadFile.tellg();

	loadFile.read((char*)&totalRows, sizeof(short));
	loadFile.read((char*)&totalCols, sizeof(short));
	
	tileMap.setDimensions(totalRows, totalCols);
	
	chtype c;
	for (unsigned int i = 0; i < totalTiles; i++)
	{
		loadFile.read((char*)&c, sizeof(chtype));
		tileMap.setDatum(i, c);
	}

	std::streampos endPos = loadFile.tellg();

	return (int)(endPos - startPos);
}


void Image::setDimensions(int rows, int cols)
{
	tileMap.setDimensions(rows, cols, ' ');
	Controllable::setDimensions(rows, cols);
}


Image::~Image()
{
	//delete tileMap;
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

	for (unsigned int i = 0; i < snapShot->getTotalTiles(); i++)
	{
		int row = i / width;
		int col = i % width;
		chtype c = mvwinch(stdscr, row + y, col + x);
		tileMap->setDatum(row, col, c);
	}

	return snapShot;
}

//This comment below could be used for an image factory method
/*	chtype c = (chtype)(row % NULL_MARKER_SPACING == 0 &&
col % NULL_MARKER_SPACING == 0) ? '!' : ' ';*/