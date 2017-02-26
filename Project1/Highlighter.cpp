#include <iostream>
#include "Highlighter.h"


Highlighter::Highlighter(Map* mapIn, short* y, short* x)
{
	map = mapIn;
	highlighting = false;
	pinPushed = false;

	win = map->getWindow();

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

	int maxY = r->y + r->height;
	int maxX = r->x + r->width;

	int containerWidth = map->getWidth();  
	int containerHeight = map->getHeight();

	maxX = r->width > containerWidth - r->x ? containerWidth : maxX;
	maxY = r->height > containerHeight - r->y ? containerHeight : maxY;

	for (int i = 0, row = r->y; row < maxY; row++, i++)
	{
		for (int j = 0, col = r->x; col < maxX; col++, j++)
		{
			map->setDisplayChar(row, col, fillChar);
		}
	}
}

void Highlighter::copy()
{
	if (highlighting == false)
		return;

	Rect* r = getHighlitRegion();
	delete cb;
	cb = new Clipboard(r->height, r->width);
	
	int maxY = r->y + r->height;
	int maxX = r->x + r->width;

	for (int i = 0, row = r->y; row < maxY; row++, i++)
	{
		for (int j = 0, col = r->x; col < maxX; col++, j++)
		{
			int bufferEl = i * cb->cols + j;
			cb->buffer[bufferEl] = map->getDisplayChar(row, col);
		}
	}
}



void Highlighter::paste()
{
	int maxY = *curY + cb->rows;
	int maxX = *curX + cb->cols;

	int containerWidth = map->getWidth();
	int containerHeight = map->getHeight();

	maxX = cb->cols > containerWidth - *curX ? containerWidth : maxX;
	maxY = cb->rows > containerHeight - *curY ? containerHeight : maxY;

	for (int i = 0, row = *curY; row < maxY; row++, i++)
	{
		for (int j = 0, col = *curX; col < maxX; col++, j++)
		{
			int bufferEl = i * cb->cols + j;
			map->setDisplayChar(row, col, cb->buffer[bufferEl]);
		}
	}

	//r = new Rect(cb->rows, cb->cols, *curY, *curX);
	pushPin(*curY + cb->rows - 1, *curX + cb->cols - 1);
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

	r->y = minY;
	r->x = minX;

	r->height = abs(*curY - piny) + 1;
	r->width = abs(*curX - pinx) + 1;

	return r;
}

void Highlighter::draw()
{
	if (highlighting)
	{
		//determine map coordinates of highlit region
		int minY = *curY < piny ? *curY : piny;
		int maxY = *curY < piny ? piny: *curY;
		int minX = *curX < pinx ? *curX : pinx;
		int maxX = *curX < pinx ? pinx : *curX;

		//translate coordinates to map location in window
		short offX = map->getUlX();
		short offY = map->getUlY();

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
}

/*
If highlighting is turned on then flip the highlit region.
If highlighting is off, then paste the buffer contents and then flip it
*/

void Highlighter::flip(int axys)
{
	//get region to flip
	Rect* r;
	if (highlighting == false)
	{
		paste();//paste buffer contents first (and turn highlighting back on)
		r = new Rect(cb->rows, cb->cols, *curY, *curX);
	}
	else
	{
		r = getHighlitRegion();
	}
	
	int maxY = r->y + r->height;
	int maxX = r->x + r->width;

	//setup axys dependent variables
	int swapCount;
	int outerMin;
	int innerMin;
	int outerMax;
	int innerMax;

	switch (axys)
	{
	case HL_HOR:
		swapCount = r->width / 2;
		outerMin = r->y;
		innerMin = r->x;
		outerMax = maxY;
		innerMax = maxX;
		break;
	case HL_VER:
		swapCount = r->height / 2;
		outerMin = r->x;
		innerMin = r->y;
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

			if (axys == HL_HOR)
			{
				swap(outer, inner, outer, right);
			}
			else
			{
				swap(inner, outer, right, outer);
			}
		}
	}
}


void Highlighter::swap(int y1, int x1, int y2, int x2)
{
	chtype swap = map->getDisplayChar(y1, x1);
	chtype rightC = map->getDisplayChar(y2, x2);
	map->setDisplayChar(y1, x1, rightC);
	map->setDisplayChar(y2, x2, swap);
}


Highlighter::~Highlighter()
{
	delete cb;
}




