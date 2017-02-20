#pragma once

#include "TUI.h"
#include "Controllable.h"
#include "Rectangle.h"
#include "Map.h"

#define HL_HOR 0
#define HL_VER 1

struct Clipboard
{
	chtype* buffer; //internal clipboard buffer
	unsigned short rows;
	unsigned short cols;
	int size;

	Clipboard(int rows, int cols)
	{
		this->rows = rows;
		this->cols = cols;
		size = rows * cols;
		buffer = new chtype[size];
	}

	void clear() { delete buffer; }
};


class Highlighter : public Controllable
{
private:
	bool highlighting;
	bool pinPushed;
	int piny, pinx; //location of pin (where highlighting originates)
	short* curY;
	short* curX; //location of cursor
	short* offY;
	short* offX;

	Clipboard* cb;
	Map* map;
	Rect* getHighlitRegion();
	void swap(int y1, int x1, int y2, int x2);

public:
	Highlighter(Map* mapIn, short* y, short* x);
	~Highlighter();
	void setHighlighting(bool status);
	void pushPin(int y, int x);
	//void setOffset(short* y, short* x);
	bool isHighlighting(){return highlighting;}
	void draw();
	void erase();
	void fill(chtype fillChar);//may be obsolete
	void copy();
	void paste();
	void flip(int axys);
};

