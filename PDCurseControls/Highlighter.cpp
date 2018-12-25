#include <iostream>
#include "Highlighter.h"


Highlighter::Highlighter(Image* imgIn, int* y, int* x)
{
	positionHighlighter(imgIn, y, x);
}


void Highlighter::positionHighlighter(Image* imgIn, int* y, int* x)
{
	img = imgIn;
	tileMap = &img->getTileMap();
	highlighting = false;
	pinPushed = false;

	win = img->getWindow();

	curY = y; //by using the address, it is always in sync with whatever is being used as the cursor
	curX = x;
}


void Highlighter::setHighlighting(bool status)
{
	highlighting = status;
	if (highlighting)
	{
		if (pinPushed == false)
			pushPin(*curY, *curX);
	}
	else
		pinPushed = false;
}

void Highlighter::erase()
{
	fill(' ');
}

void Highlighter::fill(chtype fillChar)
{
	if (highlighting == false)
		return;

	Rect* r = getHighlitRegion();

	int maxY = r->origin.y + r->height;
	int maxX = r->origin.x + r->width;

	int containerWidth = tileMap->getCols();
	int containerHeight = tileMap->getRows();

	maxX = (int)r->width > containerWidth - r->origin.x ? containerWidth : maxX;
	maxY = (int)r->height > containerHeight - r->origin.y ? containerHeight : maxY;

	for (int i = 0, row = r->origin.y; row < maxY; row++, i++)
	{
		for (int j = 0, col = r->origin.x; col < maxX; col++, j++)
		{
			tileMap->setDatum(row, col, fillChar);
		}
	}

	delete r;
}

void Highlighter::copy()
{
	if (highlighting == false)
		return;

	Rect* r = getHighlitRegion();
	clipBoard.setDimensions(r->height, r->width);
	
	int maxY = r->origin.y + r->height;
	int maxX = r->origin.x + r->width;

	for (int i = 0, row = r->origin.y; row < maxY; row++, i++)
	{
		for (int j = 0, col = r->origin.x; col < maxX; col++, j++)
		{
			chtype tile = tileMap->getDatum(row, col);
			clipBoard.setDatum(i, j, tile);
		}
	}
	delete r;
}



void Highlighter::paste()
{
	int maxY = *curY + clipBoard.getRows();
	int maxX = *curX + clipBoard.getCols();

	int containerWidth = tileMap->getCols();
	int containerHeight = tileMap->getRows();

	maxX = clipBoard.getCols() > containerWidth - *curX ? containerWidth : maxX;
	maxY = clipBoard.getRows() > containerHeight - *curY ? containerHeight : maxY;

	for (int i = 0, row = *curY; row < maxY; row++, i++)
	{
		for (int j = 0, col = *curX; col < maxX; col++, j++)
		{
			chtype tile = clipBoard.getDatum(i, j);
			tileMap->setDatum(row, col, tile);
		}
	}

	pushPin(*curY + clipBoard.getRows() - 1, *curX + clipBoard.getCols() - 1);
	highlighting = true;
}

void Highlighter::pushPin(int y, int x)
{
	piny = y;
	pinx = x;
	pinPushed = true;
}


Rect* Highlighter::getHighlitRegion()
{
	Rect* r = new Rect(); 

	int minY = *curY < piny ? *curY : piny;
	int minX = *curX < pinx ? *curX : pinx;

	r->origin.y = minY;
	r->origin.x = minX;

	r->height = abs(*curY - piny) + 1;
	r->width = abs(*curX - pinx) + 1;

	return r;
}

void Highlighter::draw()
{
	if (highlighting == false)
		return;

	//determine map coordinates of highlit region
	int minY = *curY < piny ? *curY : piny;
	int maxY = *curY < piny ? piny: *curY;
	int minX = *curX < pinx ? *curX : pinx;
	int maxX = *curX < pinx ? pinx : *curX;

	//translate coordinates to map location in window
	short offX = img->getUlX();
	short offY = img->getUlY();

	minY -= offY;
	maxY -= offY;
	minX -= offX;
	maxX -= offX;

	//make sure that new coordinates are onscreen
	int mapViewHeight = getmaxy(win);
	int mapViewWidth = getmaxx(win);

	if (minY < 0) minY = 0;
	if (minX < 0) minX = 0;
	if (maxY > mapViewHeight) maxY = mapViewHeight;
	if (maxX > mapViewWidth) maxX = mapViewWidth;

	int rows = maxY - minY;
	int cols = maxX - minX;

	//iterate through rows/cols of highlit area
	for (; minY <= maxY; minY++)
	{
		mvwchgat(win, minY, minX, cols + 1, A_STANDOUT, 0, NULL);
	}
	wnoutrefresh(win);
}


void Highlighter::flip(bool axys)
{
	//get region to flip
	Rect* r;
	if (highlighting == false)
	{
		paste();//paste buffer contents first (and turn highlighting back on)
		r = new Rect(clipBoard.getRows(), clipBoard.getCols(), Pos(*curY, *curX));
	}
	else
	{
		r = getHighlitRegion();
	}
	
	int maxY = r->origin.y + r->height;
	int maxX = r->origin.x + r->width;

	//setup axys dependent variables
	int swapCount = 0;
	int outerMin = 0;
	int innerMin = 0;
	int outerMax = 0;
	int innerMax = 0;

	switch (axys)
	{
	case AXIS_HOR:
		swapCount = r->width / 2;
		outerMin = r->origin.y;
		innerMin = r->origin.x;
		outerMax = maxY;
		innerMax = maxX;
		break;
	case AXIS_VER:
		swapCount = r->height / 2;
		outerMin = r->origin.x;
		innerMin = r->origin.y;
		outerMax = maxX;
		innerMax = maxY;
		break;
	}

	//perform flip
	for (int outer = outerMin; outer < outerMax; outer++)
	{
		for (int i = 1, inner = innerMin; i <= swapCount; inner++, i++)
		{
			int left = inner;
			int right = innerMax - i;

			if (axys == AXIS_HOR)
			{
				swap(outer, inner, outer, right);
			}
			else
			{
				swap(inner, outer, right, outer);
			}
		}
	}

	delete r;
}


void Highlighter::swap(int y1, int x1, int y2, int x2)
{
	chtype swap = tileMap->getDatum(y1, x1);
	chtype rightC = tileMap->getDatum(y2, x2);
	tileMap->setDatum(y1, x1, rightC);
	tileMap->setDatum(y2, x2, swap);
}





