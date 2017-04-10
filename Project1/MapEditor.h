#pragma once
#include "curses.h"
#include "Menu.h"
#include "Palette.h"
#include "TextField.h"
#include "TextLabel.h"
#include <iostream>
using namespace std;
#include "ControlManager.h"
#include "Frame.h"
#include "Map.h"
#include "Highlighter.h"
#include "MapEffectFilterPattern.h"
#include "MovementProcessor.h"
#include "Editor.h"

//#define NEW_MAP 0
//#define OPEN_MAP 1
//#define SAVE_MAP 2
//#define QUIT_MAP 3

#define DOT 0
#define FILL 1
#define BRUSH 2

//filter choices
#define F_NO_FILTER '-'
#define F_OBSTR 'O'
#define F_JUMPABLE 'J'
#define F_DMG_CONST 'd'
#define F_DMG_INC 'D'
#define F_AILMENT 'A'
#define F_SAVEABLE 'S'
#define F_EXIT 'E'

#define DEF_MAP_EXTENSION ".map"

class MapEditor : Editor
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
	short centerX, centerY;

	//The position of the cursor in the map
	short curX; 
	short curY;

	MovementProcessor* mp;

	Map* map;
	Image* image;
	_2DStorage<chtype>* tileMap;

	char drawChar;

	//Labels
	TextLabel* topRuler;
	TextLabel* sideRuler;
	TextLabel* textTitle;
	TextLabel* bkgdTitle;
	TextLabel* filterTitle;
	TextLabel* toolTitle;
	TextLabel* xyLbl;
	
	TextLabel* hLbl;
	TextLabel* wLbl;

	//Canvas dimension fields
	TextField* canvasRowsInput;
	TextField* canvasColsInput;

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
	MapEffectFilterPattern* mapEffectFilterPattern;
	Highlighter* hl;

	void setupPalettes();
	void setupRulers();
//	void drawRulers();
	bool processMapInput(int input);
	void processMouseInput(int input);

	void processShiftDirectionalInput(int input);

	void processPaletteInput(Palette* p, int input);
	void processFilterPaletteInput(chtype icon);
	//void processGlobalInput(int input);
	
	//Frame* createConfirmDialog();
	//void confirmDialogDriver(Controllable* dialog, int input, int confirmMethod);
	//void fileDialogDriver(Controllable* dialog, int input);
	void canvasInputDriver(TextField* field, int input);
	void setupControlManager();

	void applyTool(int y, int x);
	void createNew();
	void load(string fileName);
	void save(string fileName);
	//void setupFileDialog(int dialogType);
	void fill(int sourceRow, int sourceCol);
public:

	
	MapEditor();
	bool processInput(int input);
	void draw();

	//static callback methods
	static void paletteCallback(void*, void*, int);
	static void mapCallback(void* caller, void* ptr, int input); 
	/*static void globalCallback(void* caller, void* ptr, int input);
	static void confirmNewCallback(void* caller, void* ptr, int input); 
	static void confirmOpenCallback(void* caller, void* ptr, int input);
	static void confirmQuitCallback(void* caller, void* ptr, int input);*/
	//static void fileDialogCallback(void* caller, void* ptr, int input);
	static void canvasInputCallback(void* caller, void* ptr, int input);
};