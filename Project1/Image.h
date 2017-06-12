#pragma once

#include "curses.h"
#include "Controllable.h"
#include <iostream>
using namespace std;
#include "2DStorage.h"
#include "Storable.h"

#define NULL_MARKER_SPACING 8

class Image : public Controllable, Storable
{
private:
	//dimensional members
	unsigned int totalTiles;	
	_2DStorage<chtype>* tileMap;
	
	
	bool bordered;

	//private methods
	void init(WINDOW* win);
	void drawTileChar(
		int row, int col, //position in viewport to render char
		int mapY, int mapX); //coords of char in map array to draw
	chtype getOutOfBoundsTile(int mapX, int mapY); //a boundary drawn around the edges of the map within the viewport
public:
	Image(WINDOW* win);
	Image(int rows, int cols, WINDOW* win);

	void draw();
	void reset(); //wipe all data out of the map
	void resize(int rows, int cols);

	_2DStorage<chtype>* getTileMap() { return tileMap; }
	unsigned int getTotalTiles() { return totalTiles; };
	void setBordered(bool bordered) { this->bordered = bordered; }

	bool save(ofstream* saveFile);
	bool load(ifstream* loadFile);

	~Image();

	static Image* getScreenShot(unsigned int y, unsigned int x, unsigned int height, unsigned int width);
	static Image* getFullScreenShot();
};