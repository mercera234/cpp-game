#include <cctype>
#include <dirent.h>
#include <string>
#include "MapEditor.h"
#include "TUI.h"
#include "FileChooser.h"
#include "TwoDStorage.h"
#include "LineItem.h"
//#include "Factory.h"

const int DOT = 0;
const int FILL = 1;
const int BRUSH = 2;

//filter choices
const int F_NO_FILTER = '-';
const int F_OBSTR = 'O';
const int F_JUMPABLE = 'J';
const int F_DMG_CONST = 'd';
const int F_DMG_INC = 'D';
const int F_AILMENT = 'A';
const int F_SAVEABLE = 'S';
const int F_EXIT ='E';

MapEditor::MapEditor()
{
	topRulerRow = 2;
	sideRulerCol = 19;

	visibleRows = screenHeight;
	visibleCols = screenWidth;

	//TODO canvas default size is same as visible window? is this true?
	setCanvasSize(64, 64);

	//setup map labels
	setupRulers();


	/*when constructing the frame we add 2 extra rows and columns for the box border*/
	frame = newwin(visibleRows + 2, visibleCols + 2, topRulerRow + 1, sideRulerCol + 1);
	viewport = derwin(frame, visibleRows, visibleCols, 1, 1);

	/*set center of viewport*/
	centerY = visibleRows / 2;
	centerX = visibleCols / 2;

	curY = centerY; //center the map position to start with
	curX = centerX;
	
	setupPalettes();

	modified = false; 

	keypad(stdscr, true);

	//setup filename (this should eventually be moved to program that sets up master editor
	fileName = DEF_FILENAME;
	WINDOW* textWin = newwin(1, 15, 0, 1);
	fileNameLbl = new TextLabel(textWin, fileName);
	
	//setup default file path for opening/saving files
	char buf[256];
	GetFullPathName(".", 256, buf, NULL);
	dialogDefPath = buf;
	extensionFilter = DEF_MAP_EXTENSION;

	map.setName(fileName);
	map.setWindow(viewport);
	map.setDimensions(canvasRows, canvasCols);
	
	setConvenienceVariables();

	mp.setMoveControl(&map);
	mp.setCursor(&curY, &curX);
	mp.setViewMode(ViewMode::CENTER); //default view mode

	int bottomRow = visibleRows + 2 + topRulerRow + 1;

	xyLbl.setWindow(newwin(1, 16, bottomRow, sideRulerCol + 1));

	int hLeftEdge = sideRulerCol + 1 + 25;
	hLbl.setWindow(newwin(1, 2, bottomRow, hLeftEdge));
	hLbl.setText("H:");

	canvasRowsInput.setupField(3, bottomRow, hLeftEdge + 3);
	canvasRowsInput.setText(canvasRows);
	
	wLbl.setWindow(newwin(1, 2, bottomRow, hLeftEdge + 10));
	wLbl.setText("W:");

	canvasColsInput.setupField(3, bottomRow, hLeftEdge + 13);
	canvasColsInput.setText(canvasCols);

	resizeBtn.setWindow(newwin(1, 6, bottomRow, hLeftEdge + 18));
	resizeBtn.setText("RESIZE");
	
	
	highlighter.positionHighlighter(image, &curY, &curX);
	mapEffectFilterPattern.setMap(&map);
	mapEffectFilterPattern.setEnabled(false);

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
	cm->registerControl(&textPalette, MOUSE_LISTENER, paletteCallback);
	cm->registerControl(&bkgdPalette, MOUSE_LISTENER, paletteCallback);
	cm->registerControl(&toolPalette, MOUSE_LISTENER, paletteCallback);
	cm->registerControl(&filterPalette, MOUSE_LISTENER, paletteCallback);
	cm->registerControl(&canvasRowsInput, KEY_LISTENER | MOUSE_LISTENER, canvasInputCallback);
	cm->registerControl(&canvasColsInput, KEY_LISTENER | MOUSE_LISTENER, canvasInputCallback);
	cm->registerControl(&map, KEY_LISTENER | MOUSE_LISTENER, mapCallback);
	cm->setFocus(&map);
	cm->registerControl(&mapEffectFilterPattern, 0, 0);

	cm->registerControl(fileNameLbl, 0, 0);
	cm->registerControl(&topRuler, 0, 0);
	cm->registerControl(&xyLbl, 0, 0);
	cm->registerControl(&hLbl, 0, 0);
	cm->registerControl(&wLbl, 0, 0);
	cm->registerControl(&resizeBtn, MOUSE_LISTENER, controlCallback);
	
	cm->registerShortcutKey(KEY_ESC, globalCallback);
	cm->registerShortcutKey(CTRL_N, globalCallback);
	cm->registerShortcutKey(CTRL_S, globalCallback);
	cm->registerShortcutKey(CTRL_O, globalCallback);
}

void MapEditor::setupPalettes()
{
	//setup palette labels
	paletteLeftEdge = sideRulerCol + 1 + visibleCols + 5;

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
	textPalette.post(true);
	bkgdPalette.post(true);
	
	textColor = 0; //blank out the color(white)
	bkgdColor = 0; //blank out the color(white)

	//setup tool palette
	toolPalette.setItem("Dot", '.', 0);
	toolPalette.setItem("Fill", 'F', 1);
	toolPalette.setItem("Brush", 'B', 2);

	toolPalette.post(true);
	tool = DOT;

	//setup filter palette
	filterPalette.setItem("<no filter>", F_NO_FILTER, 0);
	filterPalette.setItem("Obstr", F_OBSTR | COLOR_PAIR(COLOR_RED_BOLD), 1);
	filterPalette.setItem("Jumpable", F_JUMPABLE | COLOR_PAIR(COLOR_CYAN_BOLD), 2);
	filterPalette.setItem("Const Dmg", F_DMG_CONST | COLOR_PAIR(COLOR_MAGENTA), 3);
	filterPalette.setItem("Inc Dmg", F_DMG_INC | COLOR_PAIR(COLOR_MAGENTA_BOLD), 4);
	filterPalette.setItem("Ailment", F_AILMENT | COLOR_PAIR(COLOR_YELLOW), 5);
	filterPalette.setItem("Save", F_SAVEABLE | COLOR_PAIR(COLOR_BLUE_BOLD), 6);
	filterPalette.setItem("Exit", F_EXIT | COLOR_PAIR(COLOR_GREEN_BOLD), 7);

	filterPalette.post(true);
}


void MapEditor::setupRulers()
{
	string topRulerText;

	//draw top ruler
	for (int i = 0; i <= visibleCols; i++)
	{
		int digit = i % 10;
		char c = digit + 48;
		topRulerText.push_back(c);
	}

	WINDOW* tRWin = newwin(1, topRulerText.length(), topRulerRow, sideRulerCol + 1);

	topRuler.setWindow(tRWin);
	topRuler.setText(topRulerText);

	/*TODO I need a vertical label to draw this component */

	//draw side ruler
	/*for (int i = 0; i <= visibleRows; i++)
	{
		int digit = i % 10;
		mvprintw(topRulerRow + 1 + i, sideRulerCol, "%d", digit);
	}*/
}


bool MapEditor::processInput(int input)
{
	cm->handleInput(input);
	return cm->isActive();
}


void MapEditor::createNew()
{
	map.reset();
	fileName = DEF_FILENAME;
	setModified(false);
	cm->setFocus(&map);
}

void MapEditor::controlCallback(void* caller, void* ptr, int input) //static
{
	MapEditor* me = (MapEditor*)caller;
	me->driver((Controllable*)ptr, input);
}

void MapEditor::driver(Controllable* control, int input)
{
	if (control == &resizeBtn)
		resizeButtonDriver(); //input is discarded, it is just the value of the mouse key
	/*else if (control == targetMenu)
		targetMenuDriver(input);
	else if (control == msgDisplay)
		displayDriver(input);*/
}



void MapEditor::paletteCallback(void* caller, void* ptr, int input) //static
{
	MapEditor* me = (MapEditor*) caller;
	me->processPaletteInput((Palette*)ptr, input);
}

void MapEditor::mapCallback(void* caller, void* ptr, int input) //static
{
	MapEditor* me = (MapEditor*)caller;
	me->processMapInput(input);
}


void MapEditor::canvasInputCallback(void* caller, void* ptr, int input) //static
{
	MapEditor* me = (MapEditor*)caller;
	me->canvasInputDriver((TextField*)ptr, input);
}

void MapEditor::canvasInputDriver(TextField* field, int input)
{
	switch (input)
	{
	case KEY_MOUSE: //cm->setFocus(field); 
		break;
	default: 
		if (isalpha(input)) break; //do not accept alphabetic characters

		field->inputChar(input);
		break;
	}



}

void MapEditor::resizeButtonDriver()
{
	int rows = stoi(canvasRowsInput.getText());
	int cols = stoi(canvasColsInput.getText());
	setCanvasSize(rows, cols);
	map.resize(canvasRows, canvasCols);

	//make sure cursor is still in bounds
	curY = curY > (short)map.getTotalRows() ? map.getTotalRows() : curY;
	curX = curX > (short)map.getTotalCols() ? map.getTotalCols() : curX;

	mp.setViewMode(ViewMode::CENTER); //reset the view to center
	setConvenienceVariables();

	cm->setFocus(&map); //switch focus back to map
}

void MapEditor::setCanvasSize(int rows, int cols)
{
	canvasRows = rows;
	canvasCols = cols;
}

void MapEditor::load(string fileName)
{
	map.load(fileName);
	
	//resize canvas to loaded image
	setConvenienceVariables();

	setCanvasSize(map.getTotalRows(), map.getTotalCols());
	
	cm->setFocus(&map);
}

void MapEditor::save(string fileName)
{
	map.save(fileName);
}


void MapEditor::processPaletteInput(Palette* p, int input)
{
	MEVENT event;
	nc_getmouse(&event);

	int pY = event.y;
	int	pX = event.x;
	wmouse_trafo(p->getWindow(), &pY, &pX, false);

	p->driver(input);
	LineItem* item = (LineItem*)p->getCurrentItem();

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
		processFilterPaletteInput(item->getIcon());
	}
}


void MapEditor::processFilterPaletteInput(chtype icon)
{
	filter = EffectType::NONE;
	int symbol = (icon & 0x0000ffff);
	switch (symbol)
	{
	case F_NO_FILTER: mapEffectFilterPattern.setEnabled(false); return; break;
	case F_OBSTR: filter = EffectType::OBSTR; break;
	case F_JUMPABLE: filter = EffectType::JUMPABLE; break;
//	case F_DMG_CONST: filter = E_HP_ALT_CONST; break;
//	case F_DMG_INC: filter = E_HP_ALT_INC; break;
	//case F_AILMENT: filter = E_AILMENT; break;
	//case F_SAVEABLE: filter = E_SAVEABLE; break;
//	case F_EXIT: filter = E_EXIT; break;
	}
	mapEffectFilterPattern.setEnabled(true);
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


bool MapEditor::processMapInput(int input)
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
	return true;
}


void MapEditor::fill(int sourceRow, int sourceCol)
{
	list<int> fillPoints;

	chtype replaceTile = image->getTile(sourceRow, sourceCol);
		//mvwinch(viewport, sourceRow, sourceCol); //make sure this isn't reversed
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
	wclear(stdscr);
	mvaddch(topRulerRow, sideRulerCol, '+');
	for (int i = 0; i <= visibleRows; i++)
	{
		int digit = i % 10;
		mvprintw(topRulerRow + 1 + i, sideRulerCol, "%d", digit);
	}
	box(frame, 0, 0);
	int bottom = getmaxy(frame);
	
	char buf[20];
	sprintf_s(buf, "x: %+4d  y: %+4d", curX, curY);
	xyLbl.setText(buf);
	
	wnoutrefresh(stdscr);
	wnoutrefresh(frame);

	
	cm->draw();
	highlighter.draw();
	chtype cursorChar = mvwinch(viewport, curY - image->getUlY(), curX - image->getUlX());
	waddch(viewport, cursorChar | A_REVERSE);
	wnoutrefresh(viewport);
}



MapEditor::~MapEditor()
{
	delwin(viewport);
	delwin(frame);
	
}