#pragma once
#include <iostream>
#include "curses.h"
#include "Controllable.h"
#include "TwoDStorage.h"
#include "TwoDStorageProxy.h"
#include "Storable.h"

class Image : public Controllable, public Storable
{
private:
	/*The array of tiles that make up the image.*/
	TwoDStorage<chtype> tileMap;
	
	//private methods
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
	void setDimensions(int rows, int cols, chtype fillChar);

	void draw();
	void setTile(unsigned int row, unsigned int col, const chtype &datum);
	chtype getTile(unsigned int row, unsigned int col);

	//storable override
	void save(std::ofstream& saveFile);
	void load(std::ifstream& loadFile);

	//getters/setters
	ITwoDStorage<chtype>& getTileMap() { return tileMap; }

	/*void setFilePath(FilePath* filePathIn) { tileMap.setFilePath(filePathIn); }
	FilePath* getFilePath() { return tileMap.getFilePath(); }*/
};

void getScreenShot(Image& image, int y, int x, int height, int width);
void getFullScreenShot(Image& image);