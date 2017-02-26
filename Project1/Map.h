#pragma once

#include "curses.h"
#include "Controllable.h"
#include <iostream>
using namespace std;


#define NULL_MARKER_SPACING 8

#define E_OBSTR 0x0001
#define E_JUMPABLE 0x0002
#define E_DMG_CONST 0x0004
#define E_DMG_INC 0x0008
#define E_AILMENT 0x0010
#define E_SAVEABLE 0x0020
#define E_EXIT 0x0040

// MSB = value, LSB = effect bitmap

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
	void resize(int rows, int cols);

	chtype* getDisplayLayer() { return displayLayer; }
	short* getEffectsLayer() { return effectsLayer; }
	void setDisplayChar(int y, int x, chtype c);
	void setEffect(int y, int x, short effect);
	void removeEffect(int y, int x, short effect);
	chtype getDisplayChar(int y, int x);
	short getEffects(int y, int x);

	string getName() { return name; }

	unsigned short getHeight() { return rows; }
	unsigned short getWidth() { return cols; }

	bool save(string fileName);
	bool load(string fileName);
	~Map();
};