#pragma once
#include <iostream>
#include "curses.h"
#include "Controllable.h"
#include "TwoDStorage.h"
#include "Storable.h"

class Image : public Controllable, public Storable
{
private:
	/*The array of tiles that make up the image.*/
	TwoDStorage<chtype> tileMap;
	
	/*
	True if a border is to be drawn around the image within the control space. 
	This is not the same thing as a frame. This is the empty space outside the image that is visible in the controls window.
	*/
	bool bordered = true;

	//private methods
	void init();
	void drawTileChar(
		unsigned short row, unsigned short col, //position in viewport to render char
		int mapY, int mapX); //coords of char in map array to draw
	chtype getOutOfBoundsTile(int mapX, int mapY); //a boundary drawn around the edges of the map within the viewport
public:
	Image();
	Image(WINDOW* win);
	Image(int rows, int cols, WINDOW* win);

	
	void reset(); //wipe all data out of the map
	void setDimensions(int rows, int cols);

	void draw();

	//Storable override
	int save(std::ofstream& saveFile);
	int load(std::ifstream& loadFile);

	void setTile(unsigned int row, unsigned int col, const chtype &datum);
	chtype getTile(unsigned int row, unsigned int col);

	//getters/setters
	TwoDStorage<chtype>* getTileMap() { return &tileMap; }
	void setBordered(bool bordered) { this->bordered = bordered; }
	bool isBordered() { return bordered; }

	static Image* getScreenShot(unsigned int y, unsigned int x, unsigned int height, unsigned int width);
	static Image* getFullScreenShot();
};