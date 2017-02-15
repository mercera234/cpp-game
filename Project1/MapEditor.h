#pragma once
#include "curses.h"
#include "Menu.h"
#include "Palette.h"
//#include "TextField.h"
#include "TextLabel.h"
#include <iostream>
using namespace std;
#include "ControlManager.h"
#include "Frame.h"
#include "Map.h"

#define NEW_MAP 0
#define OPEN_MAP 1
#define SAVE_MAP 2

#define MAP_CHOOSE 1
#define MAP_EDIT 2
#define MAP_NAME 3

#define DOT 0
#define FILL 1

#define DEF_MAP_EXTENSION ".map"
#define DEF_FILENAME "Untitled.map" //will add open/close arrows later
//#define 
class MapEditor
{
private:

	ControlManager* cm;

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

	Map* map;

	char drawChar;

	//Labels
	TextLabel* mapName;
	TextLabel* topRuler;
	TextLabel* sideRuler;
	TextLabel* textTitle;
	TextLabel* bkgdTitle;
	TextLabel* filterTitle;
	TextLabel* toolTitle;

	//what is this for?
	short layer;

	/*Palette*/
	Palette* textPalette;
	Palette* bkgdPalette;
	Palette* toolPalette;
	Palette* filterPalette;
	
	
	int paletteLeftEdge; //
	int textColor;
	int bkgdColor;
	short tool;
	int filter;

	/*global*/
	int state;
	bool modified;
	string fileName;

	void setupPalettes();
	void setupRulers();
//	void drawRulers();
	bool processMapInput(int input);
	void processMouseInput(int input);

	void processPaletteInput(Palette* p, int input);
	void processGlobalInput(int input);
	
	Frame* createConfirmDialog();
	void confirmDialogDriver(Controllable* dialog, int input, int confirmMethod);
	void fileDialogDriver(Controllable* dialog, int input);

	void setupControlManager();

	void applyTool(int y, int x);
	void newMap();
	void setupFileDialog(int dialogType);
	void fill(int sourceRow, int sourceCol);
public:

	
	MapEditor();
	bool processInput(int input);
	void loadMap();
	void draw();

	//static callback methods
	static void paletteCallback(void*, void*, int);
	static void mapCallback(void* caller, void* ptr, int input); 
	static void globalCallback(void* caller, void* ptr, int input);
	static void confirmNewCallback(void* caller, void* ptr, int input); 
	static void confirmOpenCallback(void* caller, void* ptr, int input);
	static void fileDialogCallback(void* caller, void* ptr, int input);
};