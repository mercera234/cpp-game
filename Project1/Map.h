#pragma once

#include "curses.h"
#include "Controllable.h"
#include <iostream>
using namespace std;

#define NULL_MARKER_SPACING 8

class Map : public Controllable
{
private:
	string name;
	unsigned short id;
	unsigned short rows;
	unsigned short cols;
	unsigned int totalTiles;

	unsigned short visibleRows, visibleCols; //the size of the window

	/*I read that using a 2d array is slow and inefficient, so we'll try a 1d arrays in row-major format*/
	chtype* displayLayer;
	short* effectsLayer;
	//overlays

	short ulY, ulX; //the upper left coordinate to be displayed in window
	WINDOW* win; //the window containing the visible portion of the map to be drawn

	void drawTileChar(
		int row, int col, //position in viewport to render char
		int mapY, int mapX); //coords of char in map array to draw
	chtype getOutOfBoundsTile(int mapX, int mapY); //a boundary drawn around the edges of the map within the viewport
public:
	
	Map(string name, int rows, int cols, WINDOW* win);

	void draw();
	void setPosition(int y, int x);
	void shiftVert(int magnitude);
	void shiftHor(int magnitude);
	void reset(); //wipe all data out of the map

	void setDisplayChar(int y, int x, chtype c);
	chtype getDisplayChar(int y, int x);
	string getName() { return name; }

	bool save();
	bool load(string fileName);
	~Map();
};