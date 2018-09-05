#include <cctype>
#include <dirent.h>
#include <string>
#include "MapEditor.h"
#include "TUI.h"
#include "FileChooser.h"
#include "TwoDStorage.h"
#include "LineItem.h"
#include "TextParamValue.h"

const int DOT = 0;
const int FILL = 1;
const int BRUSH = 2;

//filter choices
const int F_NO_FILTER = '-';
const int F_OBSTR = 'O';
const int F_JUMPABLE = 'J';
const int F_HP_DEC = 'd';
const int F_HP_INC = 'D';
const int F_EXIT_D = '^';
const int F_EXIT_U ='v';
const int F_SAVEABLE = 'S';

MapEditor::MapEditor()
{
	topRulerRow = 2;
	sideRulerCol = 4;

	int editorRows = 30;
	int editorCols = 100;
	resize_term(editorRows, editorCols);
	setWindow(newwin(editorRows, editorCols, 0, 0));

	fileNameLbl.setWindow(newwin(1, 15, 0, 1));
	fileNameLbl.setText(fileName);
	fileNameLbl.setFormat(new LineFormat());

	//setup default file path for opening/saving files
	char buf[256];
	GetFullPathName(".", 256, buf, NULL);
	dialogDefPath = buf;
	extensionFilter = DEF_MAP_EXTENSION;

	/*when constructing the frame we add 2 extra rows and columns for the box border*/
	mapFrame.setWindow(newwin(screenHeight + 2, screenWidth + 2, topRulerRow + 1, sideRulerCol + 1));
	mapFrame.setControl(&mapEffectFilterPattern);

	map.setName(fileName);
	map.setWindow(newwin(screenHeight, screenWidth, topRulerRow + 2, sideRulerCol + 2));
	map.setDimensions(screenHeight, screenWidth);

	setConvenienceVariables();
	
	//setup map labels
	setupRulers();

	/*set center of map*/
	centerY = screenHeight / 2;
	centerX = screenWidth / 2;

	curY = centerY; //center the map position to start with
	curX = centerX;
	
	setupPalettes();

	mp.setMoveControl(&map);
	mp.setCursor(&curY, &curX);
	mp.setViewMode(ViewMode::CENTER); //default view mode

	int bottomRow = screenHeight + 2 + topRulerRow + 1;

	coordDisplay.setWindow(newwin(1, 16, bottomRow, sideRulerCol + 1));
	//TextParamValue* xCoord = new TextParamValue(new LineFormat(0, Justf::LEFT), "x:", )


	xyLbl.setWindow(newwin(1, 16, bottomRow, sideRulerCol + 1));
	xyLbl.setFormat(new LineFormat());

	int hLeftEdge = sideRulerCol + 1 + 25;
	hLbl.setWindow(newwin(1, 2, bottomRow, hLeftEdge));
	hLbl.setText("H:");
	hLbl.setFormat(new LineFormat());

	mapRowsInput.setupField(3, bottomRow, hLeftEdge + 3);
	mapRowsInput.setText(screenHeight);
	
	wLbl.setWindow(newwin(1, 2, bottomRow, hLeftEdge + 10));
	wLbl.setText("W:");
	wLbl.setFormat(new LineFormat());

	mapColsInput.setupField(3, bottomRow, hLeftEdge + 13);
	mapColsInput.setText(screenWidth);

	resizeBtn.setWindow(newwin(1, 6, bottomRow, hLeftEdge + 18));
	resizeBtn.setText("RESIZE");
	resizeBtn.setFormat(new LineFormat());
	
	highlighter.positionHighlighter(image, &curY, &curX);
	mapEffectFilterPattern.setMap(&map);
	mapEffectFilterPattern.setEnabled(false);

	//set commands
	paletteCmd.setReceiver(this);
	paletteCmd.setAction(&MapEditor::processPaletteInput);

	canvasInputCmd.setReceiver(this);
	canvasInputCmd.setAction(&MapEditor::canvasInputDriver);

	mapCmd.setReceiver(this);
	mapCmd.setAction(&MapEditor::processMapInput);

	controlCmd.setReceiver(this);
	controlCmd.setAction(&MapEditor::driver);

	globalCmd.setReceiver(this);
	globalCmd.setAction(&MapEditor::processGlobalInput);

	newCmd.setReceiver(this);
	newCmd.setAction(&MapEditor::confirmNewDialogDriver);

	openCmd.setReceiver(this);
	openCmd.setAction(&MapEditor::confirmOpenDialogDriver);

	quitCmd.setReceiver(this);
	quitCmd.setAction(&MapEditor::confirmQuitDialogDriver);

	fileDialogCmd.setReceiver(this);
	fileDialogCmd.setAction(&MapEditor::fileDialogDriver);

	setupControlManager();
}

/*
This should be called any time a new map/image is loaded
*/
void MapEditor::setConvenienceVariables()
{
	image = map.getDisplay();
}

void MapEditor::setupControlManager()
{
	cm = new ControlManager(this);
	cm->registerControl(&textPalette, MOUSE_LISTENER, &paletteCmd);
	cm->registerControl(&bkgdPalette, MOUSE_LISTENER, &paletteCmd);
	cm->registerControl(&toolPalette, MOUSE_LISTENER, &paletteCmd);
	cm->registerControl(&filterPalette, MOUSE_LISTENER, &paletteCmd);
	cm->registerControl(&mapRowsInput, KEY_LISTENER | MOUSE_LISTENER, &canvasInputCmd);
	cm->registerControl(&mapColsInput, KEY_LISTENER | MOUSE_LISTENER, &canvasInputCmd);
	//cm->registerControl(&mapEffectFilterPattern, 0, 0);
	cm->registerControl(&fileNameLbl, 0, 0);
	cm->registerControl(&topRuler, 0, 0);
	cm->registerControl(&sideRuler, 0, 0);
	cm->registerControl(&xyLbl, 0, 0);
	cm->registerControl(&hLbl, 0, 0);
	cm->registerControl(&wLbl, 0, 0);
	cm->registerControl(&resizeBtn, MOUSE_LISTENER, &controlCmd);
	cm->registerControl(&mapFrame, KEY_LISTENER | MOUSE_LISTENER, &mapCmd);
	cm->setFocus(&mapFrame);
	
	cm->registerShortcutKey(KEY_ESC, &globalCmd);
	cm->registerShortcutKey(CTRL_N, &globalCmd);
	cm->registerShortcutKey(CTRL_S, &globalCmd);
	cm->registerShortcutKey(CTRL_O, &globalCmd);
}

void MapEditor::setupPalettes()
{
	//setup palette labels
	paletteLeftEdge = sideRulerCol + 1 + map.getVisibleCols() + 5;

	int rows = 4;
	int cols = 4;

	textPalette.setTitle("TEXT");
	textPalette.setWindows(topRulerRow, paletteLeftEdge, rows, cols);
	textPalette.setFocusable(false);

	bkgdPalette.setTitle("BKGD");
	bkgdPalette.setWindows(topRulerRow + 7, paletteLeftEdge, rows, cols);
	bkgdPalette.setFocusable(false);
	
	toolPalette.setTitle("TOOL");
	toolPalette.setWindows(topRulerRow + 14, paletteLeftEdge, 1, 3);
	toolPalette.setFocusable(false);
	
	filterPalette.setTitle("FILTER");
	filterPalette.setWindows(topRulerRow + 18, paletteLeftEdge, 2, cols);
	filterPalette.setFocusable(false);

	//setup color palettes
	for (int i = 0; i < TOTAL_COLORS; i++)
	{
		chtype c = ' ' | (i << 28) & A_COLOR;
		int x = i % cols;
		int y = i / cols;

		textPalette.setItem(colorNames[i], c, i);
		bkgdPalette.setItem(colorNames[i], c, i);
	}
	textPalette.setCurrentItem(0);
	bkgdPalette.setCurrentItem(0);
	textPalette.post(true);
	bkgdPalette.post(true);
	
	textColor = 0; //blank out the color(white)
	bkgdColor = 0; //blank out the color(white)

	//setup tool palette
	toolPalette.setItem("Dot", '.', 0);
	toolPalette.setItem("Fill", 'F', 1);
	toolPalette.setItem("Brush", 'B', 2);

	toolPalette.setCurrentItem(0);
	toolPalette.post(true);
	tool = DOT;

	//setup filter palette
	filterPalette.setItem("<no filter>", F_NO_FILTER, 0);
	filterPalette.setItem("Obstr", F_OBSTR | COLOR_PAIR(effectTypeColors[1]), 1);
	filterPalette.setItem("Jumpable", F_JUMPABLE | COLOR_PAIR(effectTypeColors[2]), 2);
	filterPalette.setItem("HP Decrease", F_HP_DEC | COLOR_PAIR(effectTypeColors[3]), 3);
	filterPalette.setItem("HP Increase", F_HP_INC | COLOR_PAIR(effectTypeColors[4]), 4);
	filterPalette.setItem("Down Exit", F_EXIT_D | COLOR_PAIR(effectTypeColors[5]), 5);
	filterPalette.setItem("Up Exit", F_EXIT_U | COLOR_PAIR(effectTypeColors[6]), 6);
	filterPalette.setItem("Saveable", F_SAVEABLE | COLOR_PAIR(effectTypeColors[7]), 7);

	filterPalette.setCurrentItem(0);
	filterPalette.post(true);
}


void MapEditor::setupRulers()
{
	std::string topRulerText = "+";

	int cols, rows;
	rows = map.getVisibleRows();
	cols = map.getVisibleCols();

	//fill top ruler with numbers from 0-9 repeatedly
	for (int i = 0; i <= cols; i++)
	{
		int digit = i % 10;
		char c = digit + 48;
		topRulerText.push_back(c);
	}
	

	WINDOW* tRWin = newwin(1, topRulerText.length(), topRulerRow, sideRulerCol);

	topRuler.setWindow(tRWin);
	topRuler.setText(topRulerText);
	topRuler.setFormat(new LineFormat);

	//setup side ruler
	std::string sideRulerText;
	for (int i = 0; i <= rows; i++)
	{
		int digit = i % 10;
		char c = digit + 48;
		sideRulerText.push_back(c);
	}

	sideRuler.setWindow(newwin(sideRulerText.length(), 1, topRulerRow + 1, sideRulerCol));
	sideRuler.setText(sideRulerText);
	sideRuler.setFormat(new VerticalLineFormat);
}


int MapEditor::processInput(int input)
{
	return cm->handleInput(input);
}


void MapEditor::createNew()
{
	map.reset();
	fileName = DEF_FILENAME;
	setModified(false);
	cm->setFocus(&mapFrame);
}


int MapEditor::driver(Controllable* control, int input)
{
	if (control == &resizeBtn)
		resizeButtonDriver(); //input is discarded, it is just the value of the mouse key
	/*else if (control == targetMenu)
		targetMenuDriver(input);
	else if (control == msgDisplay)
		displayDriver(input);*/
	return HANDLED;
}


int MapEditor::canvasInputDriver(Controllable* c, int input)
{
	TextField* field = (TextField*)c;
	switch (input)
	{
	case KEY_MOUSE: 
		break;
	default: 
		if (isalpha(input)) break; //do not accept alphabetic characters

		field->inputChar(input);
		break;
	}


	return HANDLED;
}

void MapEditor::resizeButtonDriver()
{
	setModified(true);
	int rows = stoi(mapRowsInput.getText());
	int cols = stoi(mapColsInput.getText());
	map.resize(rows, cols);

	//make sure cursor is still in bounds
	curY = curY > (short)map.getTotalRows() ? map.getTotalRows() : curY;
	curX = curX > (short)map.getTotalCols() ? map.getTotalCols() : curX;

	mp.setViewMode(ViewMode::CENTER); //reset the view to center
	setConvenienceVariables();

	cm->setFocus(&mapFrame); //switch focus back to map
}



void MapEditor::load(std::string fileName)
{
	map.load(fileName);
	
	//resize canvas to loaded image
	setConvenienceVariables();

	mapRowsInput.setText(map.getTotalRows());
	mapColsInput.setText(map.getTotalCols());

	cm->setFocus(&mapFrame);
}

void MapEditor::save(std::string fileName)
{
	map.save(fileName);
}


int MapEditor::processPaletteInput(Controllable* c, int input)
{
	MEVENT event;
	nc_getmouse(&event);

	Palette* p = (Palette*)c;

	int pY = event.y;
	int	pX = event.x;
	wmouse_trafo(p->getWindow(), &pY, &pX, false);

	p->driver(input);
	LineItem* item = (LineItem*)p->getCurrentItem();

	int exitCode = HANDLED;
	if (p == &textPalette)
	{
		textColor = (item->index << 24) & A_COLOR; //it might make more sense to use the icon, but this is backwards compatible with what I had
	}
	else if (p == &bkgdPalette)
	{
		bkgdColor = (item->index << 28) & A_COLOR; //it might make more sense to use the icon, but this is backwards compatible with what I had
	}
	else if (p == &toolPalette)
	{
		tool = item->index;
	}
	else if (p == &filterPalette)
	{
		exitCode = processFilterPaletteInput(item->getIcon());
	}
	return exitCode;
}


int MapEditor::processFilterPaletteInput(chtype icon)
{
	filter = EffectType::NONE;
	int symbol = (icon & 0x0000ffff);
	switch (symbol)
	{
	case F_NO_FILTER: mapEffectFilterPattern.setEnabled(false); return HANDLED; break;
	case F_OBSTR: filter = EffectType::OBSTR; break;
	case F_JUMPABLE: filter = EffectType::JUMPABLE; break;
	case F_HP_DEC: filter = EffectType::HP_DECREASE; break;
	case F_HP_INC: filter = EffectType::HP_INCREASE; break;
	//case F_AILMENT: filter = E_AILMENT; break;
	//case F_EXIT_D: filter = E_SAVEABLE; break;
//	case F_EXIT_U: filter = E_EXIT; break;
	}
	mapEffectFilterPattern.setEnabled(true);

	return HANDLED;
}


void MapEditor::processMouseInput(int input)
{
	MEVENT event;
	nc_getmouse(&event);
	if (event.bstate & BUTTON1_CLICKED)
	{
		int pY = event.y;
		int	pX = event.x;
		wmouse_trafo(map.getWindow(), &pY, &pX, false);

		applyTool(curY - centerY + pY, curX - centerX + pX);
	}
}


void MapEditor::applyTool(int y, int x)
{
	switch (tool)
	{
	case BRUSH:
	case DOT:
	{
		if (mapEffectFilterPattern.isEnabled())
		{
			TwoDStorage<EffectType>& el = map.getEffectsLayer();
			el.setDatum(y, x, filter);
		}
		else
		{
			chtype c = (chtype)drawChar;
			image->setTile(y, x, c | textColor | bkgdColor);
		}
	}
	break;

	case FILL:
		if (mapEffectFilterPattern.isEnabled())
		{
			break;
		}
		fill(y, x);
		break;
	}
	setModified(true);
}


void MapEditor::processShiftDirectionalInput(int input)
{
	highlighter.setHighlighting(true);
	switch (input)
	{
	case KEY_SUP: input = KEY_UP; break;
	case KEY_SDOWN: input = KEY_DOWN; break;
	case KEY_SLEFT: input = KEY_LEFT; break;
	case KEY_SRIGHT: input = KEY_RIGHT; break;
	}

	mp.processMovementInput(input);
}


int MapEditor::processMapInput(Controllable* c, int input)
{
	switch (input)
	{
	case KEY_UP:
	case KEY_DOWN:
	case KEY_LEFT:
	case KEY_RIGHT: 
	case KEY_PGUP://up down paging
	case KEY_PGDN:
	case CTL_PGUP://left right paging
	case CTL_PGDN:
	case KEY_HOME://all the way left
	case KEY_END: //all the way right
	case CTL_HOME://upper left corner
	case CTL_END: //lower right corner
		highlighter.setHighlighting(false);
		mp.processMovementInput(input);

		if (tool == BRUSH)
			applyTool(curY, curX);

		break;

	case KEY_SUP: 
	case KEY_SDOWN:
	case KEY_SLEFT:
	case KEY_SRIGHT: 
		processShiftDirectionalInput(input);
		break;

	case KEY_BACKSPACE:
	case KEY_DC:
		if (highlighter.isHighlighting())
			highlighter.erase();
		else
		{
			if (mapEffectFilterPattern.isEnabled())
			{
				map.getEffectsLayer().setDatum(curY, curX, EffectType::NONE);
			}
			else
				image->setTile(curY, curX, ' ');
		}
		break;
	case CTRL_C:
		highlighter.copy(); 
		highlighter.setHighlighting(false); 
		break;
	case CTRL_V:
		highlighter.paste(); 
		setModified(true);
		break;
	case CTL_UP:
	case CTL_DOWN:
		highlighter.flip(AXIS_VER);
		setModified(true);
		break;
	case CTL_LEFT:
	case CTL_RIGHT:
		highlighter.flip(AXIS_HOR);
		setModified(true);
		break;

	//case CTRL_B: mp.setBounded(bounded = !bounded); break;
	case CTRL_D: mp.setViewMode(ViewMode::DYNAMIC); break;
	case CTRL_L: mp.setViewMode(ViewMode::LOCK); break;
	case CTRL_P: mp.setViewMode(ViewMode::CENTER); break;

	case KEY_MOUSE:
		processMouseInput(input);
		break;
	default: //any other printable character not specified above
	 	if (input >= ' ' && input <= '~')
		{
			drawChar = input;

			if (highlighter.isHighlighting())
				highlighter.fill(drawChar | bkgdColor | textColor);
			
			applyTool(curY, curX);
		}
		
		break;
	}
	return HANDLED;
}


void MapEditor::fill(int sourceRow, int sourceCol)
{
	std::list<int> fillPoints;

	chtype replaceTile = image->getTile(sourceRow, sourceCol);
	chtype fillTile = drawChar | bkgdColor | textColor;
	if (drawChar == ' ')
	{
		fillTile = drawChar | bkgdColor; //we don't need the text color for blank spaces
	}
	
	int totalCols = map.getTotalCols();
	int totalRows = map.getTotalRows();

	if (replaceTile != fillTile) //meaning we have chosen the same fill character as what the cursor is currently over
		fillPoints.push_back(sourceRow * totalCols + sourceCol);

	int editTile;
	while (fillPoints.empty() == false)
	{
		editTile = fillPoints.front(); //consume head of queue
		fillPoints.pop_front();

		int y = editTile / totalCols;
		int x = editTile % totalCols;

		image->setTile(y, x, fillTile);

		//check adjacent positions to see if they have the replace character
		int rightTile = x + 1;
		if (rightTile < totalCols)//make sure it is on fillable map area
		{
			if(image->getTile(y, rightTile) == replaceTile)
			{
				fillPoints.push_back(y * totalCols + rightTile);
				image->setTile(y, rightTile, fillTile);
			}
		}

		int leftTile = x - 1;
		if (leftTile >= 0)//make sure it is on fillable map area
		{
			if (image->getTile(y, leftTile) == replaceTile)
			{
				fillPoints.push_back(y * totalCols + leftTile);
				image->setTile(y, leftTile, fillTile);
			}
		}

		int downTile = y + 1;
		if (downTile < totalRows)//make sure it is on fillable map area
		{
			if (image->getTile(downTile, x) == replaceTile)
			{
				fillPoints.push_back(downTile * totalCols + x);
				image->setTile(downTile, x, fillTile);
			}
		}
		
		int upTile = y - 1;
		if (upTile >= 0)//make sure it is on fillable map area
		{
			if (image->getTile(upTile, x) == replaceTile)
			{
				fillPoints.push_back(upTile * totalCols + x);
				image->setTile(upTile, x, fillTile);
			}
		}
	}

	fillPoints.clear();
}

void MapEditor::draw()
{
	wclear(win);
	wnoutrefresh(win);

	char buf[20];
	sprintf_s(buf, "x: %+4d  y: %+4d", curX, curY);
	xyLbl.setText(buf);
	
	cm->draw();
	highlighter.draw();

	WINDOW* mapWin = map.getWindow();
	chtype cursorChar = mvwinch(mapWin, curY - image->getUlY(), curX - image->getUlX());
	waddch(mapWin, cursorChar | A_REVERSE);
	wnoutrefresh(mapWin);
}



MapEditor::~MapEditor()
{
}