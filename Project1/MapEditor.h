#pragma once
#include "curses.h"
#include "Palette.h"
#include "TextField.h"
#include "TextLabel.h"
#include "ControlManager.h"
#include "Frame.h"
#include "Map.h"
#include "Highlighter.h"
#include "MapEffectFilterPattern.h"
#include "FreeMovementProcessor.h"
#include "Editor.h"

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
	int curX; 
	int curY;

	FreeMovementProcessor mp;

	Map map;
	Image* image;//a convenience pointer to the map's image

	char drawChar;

	//Labels
	TextLabel topRuler;
	TextLabel sideRuler;
	TextLabel xyLbl;
	
	TextLabel hLbl;
	TextLabel wLbl;
	TextLabel resizeBtn;

	//Canvas dimension fields
	TextField canvasRowsInput;
	TextField canvasColsInput;

	//what is this for?
	short layer;

	/*Palette*/
	Palette textPalette;
	Palette bkgdPalette;
	Palette toolPalette;
	Palette filterPalette;
	
	int paletteLeftEdge; //
	int textColor;
	int bkgdColor;
	short tool;
	EffectType filter;

	MapEffectFilterPattern mapEffectFilterPattern;
	Highlighter highlighter;

	void setupPalettes();
	void setupRulers();
	void setCanvasSize(int rows, int cols);
	void setConvenienceVariables();

	bool processMapInput(int input);
	void processMouseInput(int input);

	void processShiftDirectionalInput(int input);

	void processPaletteInput(Palette* p, int input);
	void processFilterPaletteInput(chtype icon);
	void resizeButtonDriver();
	
	void canvasInputDriver(TextField* field, int input);
	void setupControlManager();

	void applyTool(int y, int x);
	void createNew();
	void load(string fileName);
	void save(string fileName);
	
	void fill(int sourceRow, int sourceCol);

	void driver(Controllable* control, int input);
public:

	MapEditor();
	~MapEditor();
	bool processInput(int input);
	void draw();

	//static callback methods
	static void paletteCallback(void*, void*, int);
	static void mapCallback(void* caller, void* ptr, int input); 
	static void canvasInputCallback(void* caller, void* ptr, int input);
	static void controlCallback(void*, void*, int);
};