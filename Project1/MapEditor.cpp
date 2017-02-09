#include "MapEditor.h"
#include "TUI.h"
//#include "MouseHelper.h"
#include "Frame.h"

MapEditor::MapEditor()
{
	canvasRows = 64;
	canvasCols = 64;

	topRulerRow = 2;
	sideRulerCol = 19;

	visibleRows = 23;
	visibleCols = 51;

	//setup map labels
	setupRulers();


	/*when constructing the frame we add 2 extra rows and columns for the box border*/
	frame = newwin(visibleRows + 2, visibleCols + 2, topRulerRow + 1, sideRulerCol + 1);
	viewport = derwin(frame, visibleRows, visibleCols, 1, 1);

	
	/*set center of viewport*/
	centerY = visibleRows / 2;
	centerX = visibleCols / 2;

	y = centerY; //center the map position to start with
	x = centerX;
	
	
	
	setupPalettes();

	state = MAP_EDIT;

	modified = false; 

	keypad(stdscr, true);

	//setup filename
	fileName = DEF_FILENAME;
	WINDOW* textWin = newwin(1, 15, 0, 1);
	mapName = new TextLabel(textWin, fileName);
	
	
	map = new Map(fileName, canvasRows, canvasCols, viewport);

	setupControlManager();
}

void MapEditor::setupControlManager()
{
	cm = new ControlManager(this);
	cm->registerControl(textPalette, MOUSE_LISTENER, paletteCallback);
	cm->registerControl(bkgdPalette, MOUSE_LISTENER, paletteCallback);
	cm->registerControl(toolPalette, MOUSE_LISTENER, paletteCallback);
	cm->registerControl(filterPalette, MOUSE_LISTENER, paletteCallback);
	cm->registerControl(map, KEY_LISTENER | MOUSE_LISTENER, mapCallback);
	cm->setFocus(map);
	cm->registerControl(mapName, 0, 0);
	cm->registerControl(topRuler, 0, 0);
	cm->registerControl(textTitle, 0, 0);
	cm->registerControl(bkgdTitle, 0, 0);
	cm->registerControl(toolTitle, 0, 0);
	cm->registerControl(filterTitle, 0, 0);

	cm->registerShortcutKey(CTRL_Q, globalCallback);
	cm->registerShortcutKey(CTRL_N, globalCallback);
	cm->registerShortcutKey(CTRL_S, globalCallback);
	cm->registerShortcutKey(CTRL_O, globalCallback);
}

void MapEditor::setupPalettes()
{
	//setup palette labels
	paletteLeftEdge = sideRulerCol + 1 + visibleCols + 5;

	WINDOW* tLwin = newwin(1, 4, topRulerRow, paletteLeftEdge);
	textTitle = new TextLabel(tLwin, "TEXT");

	WINDOW* bLwin = newwin(1, 4, topRulerRow + 7, paletteLeftEdge);
	bkgdTitle = new TextLabel(bLwin, "BKGD");

	WINDOW* toLwin = newwin(1, 4, topRulerRow + 14, paletteLeftEdge);
	toolTitle = new TextLabel(toLwin, "TOOL");

	WINDOW* fLwin = newwin(1, 6, topRulerRow + 18, paletteLeftEdge);
	filterTitle = new TextLabel(fLwin, "FILTER");



	//setup color palettes
	int rows = 4;
	int cols = 4;
	textPalette = new Palette(rows, cols, topRulerRow + 1, paletteLeftEdge);
	bkgdPalette = new Palette(rows, cols, topRulerRow + 8, paletteLeftEdge);

	for (int i = 0; i < TOTAL_COLORS; i++)
	{
		chtype c = ' ' | (i << 28) & A_COLOR;
		int x = i % cols;
		int y = i / cols;
		textPalette->setItem(TUI::colorNames[i], c, y, x);
		bkgdPalette->setItem(TUI::colorNames[i], c, y, x);
	}

	textColor = 0; //blank out the color(white)
	bkgdColor = 0; //blank out the color(white)

	//setup tool palette
	toolPalette = new Palette(1, 2, topRulerRow + 15, paletteLeftEdge);
	toolPalette->setItem("Dot", 'B', 0, 0);
	toolPalette->setItem("Fill", 'F', 0, 1);
	tool = DOT;

	//setup filter palette
	filterPalette = new Palette(2, 4, topRulerRow + 19, paletteLeftEdge);

	filterPalette->setItem("<no filter>", '-', 0, 0);
	filterPalette->setItem("Obstr", 'O', 0, 1);
	filterPalette->setItem("Jumpable", 'J', 0, 2);
	filterPalette->setItem("Const Dmg", 'd', 0, 3);
	filterPalette->setItem("Inc Dmg", 'D', 1, 0);
	filterPalette->setItem("Ailment", 'A', 1, 1);
	filterPalette->setItem("Save", 'S', 1, 2);
	filterPalette->setItem("Exit", 'E', 1, 3);
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
	topRuler = new TextLabel(tRWin, topRulerText);

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
	return cm->handleInput(input);
}




void MapEditor::processGlobalInput(int input)
{
	//handle preliminary input
	switch (input)
	{
	case CTRL_Q: //use to quit, will add more logic eventually to prevent user from quitting without saving
		cm->prepareForShutdown();
		break;
	case CTRL_N: 
		if (modified)
		{
			//create confirmation dialog
			string confirmMsg = "Are you sure? You have unsaved changes.";

			int width, height;  getmaxyx(stdscr, height, width);
			WINDOW* fWin = newwin(4, confirmMsg.length() + 2, (height - 4) / 2, (width - 40) / 2);
			WINDOW* cdWin = derwin(fWin, 1, confirmMsg.length(), 2, 1);
			Menu* cdMenu = new Menu(cdWin, 1, 2);
			//cdMenu->setModal(true);
			cdMenu->setItem("No", "", 0, 0);
			cdMenu->setItem("Yes", "", 1, 1);

			Frame* f = new Frame(fWin, cdMenu);
			f->setText(confirmMsg, 1, 1);
			f->setModal(true);

			cm->registerControl(f, KEY_LISTENER, dialogCallback);
			cm->setFocus(f);
		}
		else
		{
			newMap();
		}
		break;
	case CTRL_O:
		map->load("Untitled.map");
		break;
	case CTRL_S: 
		map->save();
		break;
	}
}


void MapEditor::newMap()
{
	map->reset();
	modified = false;
	mapName->setText(DEF_FILENAME);
}

void MapEditor::globalCallback(void* caller, void* ptr, int input) //static
{
	MapEditor* me = (MapEditor*)caller;
	me->processGlobalInput(input);
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

void MapEditor::dialogCallback(void* caller, void* ptr, int input) //static
{
	MapEditor* me = (MapEditor*)caller;
	me->confirmDialogDriver((Controllable*)ptr, input);
}


void MapEditor::confirmDialogDriver(Controllable* c, int input)
{
	Frame* f = (Frame*)c;
	Menu* dialog = (Menu*)f->getControl();

	switch (input)
	{
	case KEY_LEFT: dialog->driver(REQ_LEFT_ITEM); break;
	case KEY_RIGHT: dialog->driver(REQ_RIGHT_ITEM); break;
	case '\r': dialog->driver(REQ_TOGGLE_ITEM); break;
	}

	MenuItem* mi = dialog->getSelectedItem();
	if (mi != NULL)
	{
		switch (mi->index)
		{
		case 0: //no
			cm->popControl();
			cm->setFocus(map);
			break;
		case 1: //yes
			newMap();
			cm->popControl();
			cm->setFocus(map);
			break;
		}
	}
}


void MapEditor::processPaletteInput(Palette* p, int input)
{
	int pY, pX;
	MEVENT event;
	nc_getmouse(&event);
	p->translateCoords(event.y, event.x, pY, pX);

	PaletteItem* item = p->pickItem(pY, pX);

	if (p == textPalette)
	{
		textColor = (item->index << 24) & A_COLOR; //it might make more sense to use the icon, but this is backwards compatible with what I had
	}
	else if (p == bkgdPalette)
	{
		bkgdColor = (item->index << 28) & A_COLOR; //it might make more sense to use the icon, but this is backwards compatible with what I had
	}
	else if (p == toolPalette)
	{
		tool = item->index;
	}
	else if (p == filterPalette)
	{
		filter = item->index; 
	}
}

void MapEditor::processMouseInput(int input)
{
	MEVENT event;
	nc_getmouse(&event);
	if (event.bstate & BUTTON1_CLICKED)
	{
		int pY, pX;
		map->translateCoords(event.y, event.x, pY, pX);

		applyTool(y - centerY + pY, x - centerX + pX);
	}
}


void MapEditor::applyTool(int y, int x)
{
	switch (tool)
	{
	case DOT:
	{
		chtype c = (chtype)drawChar;
		map->setDisplayChar(y, x, c | textColor | bkgdColor);
	}
	break;

	case FILL:
		fill(y, x);
		break;
	}
	modified = true;
	mapName->setText(fileName + "*");
}

bool MapEditor::processMapInput(int input)
{
	switch (input)
	{
	case KEY_UP:
		if (y - 1 >= 0)
		{
			y--;
			map->shiftVert(-1);
		}
		break;
	case KEY_DOWN:
		if (y + 1 < canvasRows)
		{
			y++;
			map->shiftVert(1);
		}
		break;
	case KEY_LEFT:
		if (x - 1 >= 0)
		{
			x--;
			map->shiftHor(-1);
		}
		break;
	case KEY_RIGHT:
		if (x + 1 < canvasCols)
		{
			x++;
			map->shiftHor(1);
		}			
		break;
	case KEY_BACKSPACE:
	case KEY_DC:
		map->setDisplayChar(y, x, ' ');
		break;
	case KEY_MOUSE:
		processMouseInput(input);
		break;
	default: //any other printable character not specified above
	 	if (input >= ' ' && input <= '~')
		{
			drawChar = input;
			applyTool(y,x);
		}
		
		break;
	}
	return true;
}


void MapEditor::fill(int sourceRow, int sourceCol)
{
	list<int> fillPoints;

	chtype replaceTile = map->getDisplayChar(sourceRow, sourceCol);
		//mvwinch(viewport, sourceRow, sourceCol); //make sure this isn't reversed
	chtype fillTile = drawChar | bkgdColor | textColor;
	if (drawChar == ' ')
	{
		fillTile = drawChar | bkgdColor; //we don't need the text color for blank spaces
	}
	
	int totalCols = map->getWidth();
	int totalRows = map->getHeight();

	if (replaceTile != fillTile) //meaning we have chosen the same fill character as what the cursor is currently over
		fillPoints.push_back(sourceRow * totalCols + sourceCol);

	int editTile;
	while (fillPoints.empty() == false)
	{
		editTile = fillPoints.front(); //consume head of queue
		fillPoints.pop_front();

		int y = editTile / totalCols;
		int x = editTile % totalCols;

		map->setDisplayChar(y, x, fillTile);

		//check adjacent positions to see if they have the replace character
		int rightTile = x + 1;
		if (rightTile < totalCols)//make sure it is on fillable map area
		{
			if(map->getDisplayChar(y, rightTile) == replaceTile)
			{
				fillPoints.push_back(y * totalCols + rightTile);
				map->setDisplayChar(y, rightTile, fillTile);
			}
		}

		int leftTile = x - 1;
		if (leftTile >= 0)//make sure it is on fillable map area
		{
			if (map->getDisplayChar(y, leftTile) == replaceTile)
			{
				fillPoints.push_back(y * totalCols + leftTile);
				map->setDisplayChar(y, leftTile, fillTile);
			}
		}

		int downTile = y + 1;
		if (downTile < totalRows)//make sure it is on fillable map area
		{
			if (map->getDisplayChar(downTile, x) == replaceTile)
			{
				fillPoints.push_back(downTile * totalCols + x);
				map->setDisplayChar(downTile, x, fillTile);
			}
		}
		
		int upTile = y - 1;
		if (upTile >= 0)//make sure it is on fillable map area
		{
			if (map->getDisplayChar(upTile, x) == replaceTile)
			{
				fillPoints.push_back(upTile * totalCols + x);
				map->setDisplayChar(upTile, x, fillTile);
			}
		}
	}

	fillPoints.clear();
}


void MapEditor::loadMap()
{
	
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
	mvprintw(bottom + topRulerRow + 1, sideRulerCol + 1, "x: %+4d  y: %+4d", x, y); //%+4d always render sign, 4 char field, int

	
	wnoutrefresh(stdscr);
	wnoutrefresh(frame);
	cm->draw();

	chtype cursorChar = mvwinch(viewport, centerY, centerX);
	waddch(viewport, cursorChar | A_REVERSE);
	wnoutrefresh(viewport);
}



