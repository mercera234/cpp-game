#pragma once
#include "curses.h"
#include "Palette.h"
#include "TextField.h"
#include "TextLabel.h"
#include "ControlManager.h"
#include "Frame.h"
#include "MapRoom.h"
#include "Highlighter.h"
#include "MapEffectFilterPattern.h"
#include "FreeMovementProcessor.h"
#include "Editor.h"
#include "SimpleControlCommand.h"
#include "TextBoard.h"
#include "EditMode.h"



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
	
	//convenience vars that already exist in the different mode types
	MapRoom* room;
	Image* image;

	char drawChar;

	//Labels
	TextLabel topRuler;
	TextLabel sideRuler;
	
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

	
	MapRoomEditMode mapRoomEditMode;
	ImageEditMode imageEditMode;
	
	void setupPalettes();
	void setupRulers();
	void setupCommands();
	
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
	void load(const std::string& fileName);
	void save(const std::string& fileName);
	void cycleMode();
	
	void fill(int sourceRow, int sourceCol);

	int driver(Controllable* control, int input);
public:

	MapEditor();
	~MapEditor();
	int processInput(int input);
	void draw();
};