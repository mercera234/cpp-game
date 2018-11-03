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
#include "SimpleCommand.h"
#include "TextBoard.h"
#include "EditMode.h"
#include "FormField.h"


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
	
	FormField rowField;
	FormField colField;

	TextLabel resizeBtn;


	//what is this for?
//	short layer;

	/*Palette*/
	Palette textPalette;
	Palette bkgdPalette;
	Palette toolPalette;
	Palette filterPalette; //map room mode only
	Palette layerPalette; //megamap mode only
	
	int paletteLeftEdge; //
	int column2Edge; //left edge of second column past color palettes
	int textColor;
	int bkgdColor;
	short tool;
	EffectType filter;

	MapEffectFilterPattern mapEffectFilterPattern;
	Highlighter highlighter;

	SimpleCommand<MapEditor> paletteCmd;
	SimpleCommand<MapEditor> mapCmd;
	SimpleCommand<MapEditor> canvasInputCmd;
	SimpleCommand<MapEditor> controlCmd;
	SimpleCommand<MapEditor> globalCmd;
	

	MapRoomEditMode mapRoomEditMode;
	ImageEditMode imageEditMode;
	//MegaMapEditMode megaMapEditMode;


	void setupPalettes();
	void setupRulers();
	void setupCommands();
	
	void processMapInput();
	void processMouseInput(int input);

	void processShiftDirectionalInput(int input);

	void processPaletteInput();
	void processFilterPaletteInput(chtype icon);
	void resizeButtonDriver();
	
	void canvasInputDriver();
	void setupControlManager();

	void modeDriver();

	void applyTool(int y, int x);
	void createNew();
	void load(const std::string& fileName);
	void save(const std::string& fileName);
	
	void fill(int sourceRow, int sourceCol);

	void driver();
public:

	MapEditor();
	~MapEditor();
	int processInput(int input);
	void draw();
};