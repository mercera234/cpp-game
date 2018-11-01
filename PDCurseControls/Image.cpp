#include <fstream>
#include <assert.h>
#include "Image.h"
#include "TUI.h"

const int NULL_MARKER_SPACING = 8;

Image::Image()
{
}


Image::Image(WINDOW* win)
{
	setWindow(win);
}

Image::Image(int rows, int cols, WINDOW* win)
{
	setWindow(win);

	setDimensions(rows, cols);	
}


void Image::reset()
{
	tileMap.fill(' ');
}


void Image::draw()
{
	if (win == nullptr) //can't draw without a space to draw in
		return;

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
	touchwin(win);
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


void Image::save(std::ofstream& saveFile)
{
	tileMap.save(saveFile);
}

void Image::load(std::ifstream& loadFile)
{
	tileMap.load(loadFile);
	Controllable::setDimensions(tileMap.getRows(), tileMap.getCols());
}

void Image::setDimensions(int rows, int cols)
{
	tileMap.setDimensions(rows, cols);
	Controllable::setDimensions(rows, cols);
}

void Image::setDimensions(int rows, int cols, chtype fillChar)
{
	tileMap.setDimensions(rows, cols, fillChar);
	Controllable::setDimensions(rows, cols);
}


void Image::setTile(unsigned int row, unsigned int col, const chtype &datum)
{
	tileMap.setDatum(row, col, datum);
}

chtype Image::getTile(unsigned int row, unsigned int col)
{
	return tileMap.getDatum(row, col);
}


void getFullScreenShot(Image& snapShot) //static
{
	return getScreenShot(snapShot, 0, 0, getmaxy(stdscr), getmaxx(stdscr));
}

void getScreenShot(Image& snapShot, int y, int x, int height, int width) //static
{
	int winheight = getmaxy(stdscr);
	int winWidth = getmaxx(stdscr);

	assert(x >= 0 && y >= 0 && x < winWidth && y < winheight);
		

	//if width/height was set too long, then take a snapshot of what is available
	if (x + width > winWidth)
		width = winWidth - x;

	if (y + height > winheight)
		height = winheight - y;


	//create new snapshot
	snapShot.setDimensions(height, width);
	snapShot.setWindow(stdscr);

	ITwoDStorage<chtype>& tileMap = snapShot.getTileMap();

	for (int i = 0; i < snapShot.getTotalTiles(); i++)
	{
		int row = i / width;
		int col = i % width;
		chtype c = mvwinch(stdscr, row + y, col + x);
		tileMap.setDatum(row, col, c);
	}
}
