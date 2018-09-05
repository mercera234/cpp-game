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
#include "SimpleControlCommand.h"
#include "TextBoard.h"

const std::string DEF_MAP_EXTENSION = ".map";

class MapEditor : Editor<MapEditor>
{
private:
	/*these vars identify the top left corner above and left of the viewport window*/
	int topRulerRow;
	int sideRulerCol;

	/*The cursor position in the viewport*/
	short centerX, centerY;

	//The position of the cursor in the map
	int curX; 
	int curY;

	FreeMovementProcessor mp;

	Frame mapFrame;
	Map map;
	Image* image;//a convenience pointer to the map's image

	char drawChar;

	//Labels
	TextLabel topRuler;
	TextLabel sideRuler;
	TextLabel xyLbl;

	TextBoard coordDisplay;
	
	TextLabel hLbl;
	TextLabel wLbl;
	TextLabel resizeBtn;

	//Canvas dimension fields
	TextField mapRowsInput;
	TextField mapColsInput;

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

	SimpleControlCommand<MapEditor> paletteCmd;
	SimpleControlCommand<MapEditor> mapCmd;
	SimpleControlCommand<MapEditor> canvasInputCmd;
	SimpleControlCommand<MapEditor> controlCmd;
	SimpleControlCommand<MapEditor> globalCmd;

	void setupPalettes();
	void setupRulers();
	//void setCanvasSize(int rows, int cols);
	void setConvenienceVariables();

	int processMapInput(Controllable* c, int input);
	void processMouseInput(int input);

	void processShiftDirectionalInput(int input);

	int processPaletteInput(Controllable* c, int input);
	int processFilterPaletteInput(chtype icon);
	void resizeButtonDriver();
	
	int canvasInputDriver(Controllable* c, int input);
	void setupControlManager();

	void applyTool(int y, int x);
	void createNew();
	void load(std::string fileName);
	void save(std::string fileName);
	
	void fill(int sourceRow, int sourceCol);

	int driver(Controllable* control, int input);
public:

	MapEditor();
	~MapEditor();
	int processInput(int input);
	void draw();
};