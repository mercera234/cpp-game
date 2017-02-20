#pragma once

#include "curses.h"
#include "Controllable.h"
#include <iostream>
using namespace std;

#define NULL_MARKER_SPACING 8

class Map : public Controllable
{
private:
	//meta members
	string name;
	unsigned short id;

	//dimensional members
	unsigned short rows;
	unsigned short cols;
	unsigned int totalTiles;

	unsigned short visibleRows, visibleCols; //the size of the window
	short ulY, ulX; //the upper left coordinate to be displayed in window
	short mapY, mapX; //the coordinates of the upper left corner of the map relative to the view

	
	//data storage
	/*I read that using a 2d array is slow and inefficient, so we'll try a 1d arrays in row-major format*/
	chtype* displayLayer;
	short* effectsLayer;
	
	//private methods
	void drawTileChar(
		int row, int col, //position in viewport to render char
		int mapY, int mapX); //coords of char in map array to draw
	chtype getOutOfBoundsTile(int mapX, int mapY); //a boundary drawn around the edges of the map within the viewport
public:
	chtype filterMask;//a mask that causes the map to be drawn without certain attributes (so we can draw a black and white map if we want)

	Map(string name, int rows, int cols, WINDOW* win);

	void draw();
	void setPosition(int y, int x);
	void shift(int y, int x);
	void reset(); //wipe all data out of the map
	

	chtype* getDisplayLayer() { return displayLayer; }
	short* getEffectsLayer() { return effectsLayer; }
	void setDisplayChar(int y, int x, chtype c);
	chtype getDisplayChar(int y, int x);
	string getName() { return name; }

	short getUlY() { return ulY; }
	short getUlX() { return ulX; }
	unsigned short getHeight() { return rows; }
	unsigned short getWidth() { return cols; }

	bool save(string fileName);
	bool load(string fileName);
	~Map();
};