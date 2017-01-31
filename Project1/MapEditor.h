#pragma once
#include "curses.h"
#include "Menu.h"
#include "ColorPalette.h"
#include "Palette.h"
#include "TextField.h"
#include <iostream>
using namespace std;

#include "Map.h"



#define MAP_CHOOSE 1
#define MAP_EDIT 2
#define MAP_NAME 3

//#define 
class MapEditor
{
private:
	/*Map space*/
	int canvasRows; //!!!these vars are deprecated and should be used from map object
	int canvasCols;

	/*these vars identify the top left corner above and left of the viewport window*/
	int topRulerRow;
	int sideRulerCol;

	WINDOW* frame;
	WINDOW* viewport; //this is now in the map, but I'm keeping the reference here in case it is useful for drawing other layers over the map
	int visibleRows;
	int visibleCols;

	/*The cursor position in the viewport*/
	int centerX, centerY;
	
	/*The position in the map the cursor is over*/
	int x;
	int y;

	//stub!! a fake map variable, will replace with a real one later
	//chtype** map;
	Map* map;

	//map name editor
	TextField* mapName;

	short layer;

	/*Palette*/
	Palette* textPalette;
	Palette* bkgdPalette;
	Palette* filterPalette;
	
	int paletteLeftEdge; //
	int textColor;
	int bkgdColor;
	int filter;

	/*global*/
	int state;
	bool unsavedChanges;
	string fileName;


	void setupPalettes();
	void drawRulers();
	bool processMapInput(int input);
	void processMouseInput(int input);
public:

	
	MapEditor();
	bool processInput(int input);
	void loadMap();
	void draw();


};