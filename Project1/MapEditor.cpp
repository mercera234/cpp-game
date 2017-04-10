#include "MapEditor.h"
#include "TUI.h"
#include "FileChooser.h"
#include <dirent.h>
#include <string>
#include "2DStorage.h"
#include "LineItem.h"

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

	
	map = new Map(fileName, canvasRows, canvasCols, viewport);
	
	image = map->getDisplay();
	tileMap = image->getTileMap();

	mp = new MovementProcessor(image, &curY, &curX);
	int bottomRow = visibleRows + 2 + topRulerRow + 1;

	//mvprintw(bottom + topRulerRow + 1, sideRulerCol + 1, "x: %+4d  y: %+4d", x, y); //%+4d always render sign, 4 char field, int
	xyLbl = new TextLabel(newwin(1, 16, bottomRow, sideRulerCol + 1), "");

	int hLeftEdge = sideRulerCol + 1 + 25;
	hLbl = new TextLabel(newwin(1, 2, bottomRow, hLeftEdge), "H:");
	canvasRowsInput = new TextField(newwin(1, 3, bottomRow, hLeftEdge + 3));
	canvasRowsInput->setText(canvasRows);
	

	wLbl = new TextLabel(newwin(1, 2, bottomRow, hLeftEdge + 10), "W:");
	canvasColsInput = new TextField(newwin(1, 3, bottomRow, hLeftEdge + 13));
	canvasColsInput->setText(canvasCols);

	
	//hl = new Highlighter(map->getWindow(), map->getDisplayLayer(), &centerY, &centerX); //map->getMapY(), map->getMapX());
	hl = new Highlighter(image, &curY, &curX);
	mapEffectFilterPattern = new MapEffectFilterPattern(map);
	mapEffectFilterPattern->setEnabled(false);

	setupControlManager();
}

void MapEditor::setupControlManager()
{
	cm = new ControlManager(this);
	cm->registerControl(textPalette, MOUSE_LISTENER, paletteCallback);
	cm->registerControl(bkgdPalette, MOUSE_LISTENER, paletteCallback);
	cm->registerControl(toolPalette, MOUSE_LISTENER, paletteCallback);
	cm->registerControl(filterPalette, MOUSE_LISTENER, paletteCallback);
	cm->registerControl(canvasRowsInput, KEY_LISTENER | MOUSE_LISTENER, canvasInputCallback);
	cm->registerControl(canvasColsInput, KEY_LISTENER | MOUSE_LISTENER, canvasInputCallback);
	cm->registerControl(map, KEY_LISTENER | MOUSE_LISTENER, mapCallback);
	cm->setFocus(map);
	cm->registerControl(mapEffectFilterPattern, 0, 0);
	cm->registerControl(hl, 0, 0);
	cm->registerControl(fileNameLbl, 0, 0);
	cm->registerControl(topRuler, 0, 0);
	cm->registerControl(textTitle, 0, 0);
	cm->registerControl(bkgdTitle, 0, 0);
	cm->registerControl(toolTitle, 0, 0);
	cm->registerControl(filterTitle, 0, 0);
	cm->registerControl(xyLbl, 0, 0);
	cm->registerControl(hLbl, 0, 0);
	cm->registerControl(wLbl, 0, 0);
	
	cm->registerShortcutKey(KEY_ESC, globalCallback);
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
	toolPalette = new Palette(1, 3, topRulerRow + 15, paletteLeftEdge);
	toolPalette->setItem("Dot", '.', 0, 0);
	toolPalette->setItem("Fill", 'F', 0, 1);
	toolPalette->setItem("Brush", 'B', 0, 2);
	tool = DOT;

	//setup filter palette
	filterPalette = new Palette(2, 4, topRulerRow + 19, paletteLeftEdge);

	filterPalette->setItem("<no filter>", F_NO_FILTER, 0, 0);
	filterPalette->setItem("Obstr", F_OBSTR | COLOR_PAIR(COLOR_RED_BOLD) , 0, 1);
	filterPalette->setItem("Jumpable", F_JUMPABLE | COLOR_PAIR(COLOR_CYAN_BOLD), 0, 2);
	filterPalette->setItem("Const Dmg", F_DMG_CONST | COLOR_PAIR(COLOR_MAGENTA), 0, 3);
	filterPalette->setItem("Inc Dmg", F_DMG_INC | COLOR_PAIR(COLOR_MAGENTA_BOLD), 1, 0);
	filterPalette->setItem("Ailment", F_AILMENT | COLOR_PAIR(COLOR_YELLOW), 1, 1);
	filterPalette->setItem("Save", F_SAVEABLE | COLOR_PAIR(COLOR_BLUE_BOLD), 1, 2);
	filterPalette->setItem("Exit", F_EXIT | COLOR_PAIR(COLOR_GREEN_BOLD), 1, 3);
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


void MapEditor::createNew()
{
	map->reset();
	fileName = DEF_FILENAME;
	setModified(false);
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
	case '\t': 
		//reset dimensions on map !!!this may just be a stub, I may want to add a button to update later
		canvasRows = stoi(canvasRowsInput->getText());
		canvasCols = stoi(canvasColsInput->getText());
		map->resize(canvasRows, canvasCols);

		cm->setFocus(map); //switch focus back to map
		break;
	default: 
		field->inputChar(input);
		break;
	}



}


void MapEditor::load(string fileName)
{
	map->load(fileName);
}

void MapEditor::save(string fileName)
{
	map->save(fileName);
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
		filter = item->index; //do I need this?
		processFilterPaletteInput(item->icon);
	}
}


void MapEditor::processFilterPaletteInput(chtype icon)
{
	int color = (icon & 0x0f000000) >> 24;
	int filter = 0;
	int symbol = icon & 0x0000ffff;
	switch (symbol)
	{
	case F_NO_FILTER: mapEffectFilterPattern->setEnabled(false); return; break;
	case F_OBSTR: filter = E_OBSTR; break;
	case F_JUMPABLE: filter = E_JUMPABLE; break;
	case F_DMG_CONST: filter = E_HP_ALT_CONST; break;
//	case F_DMG_INC: filter = E_HP_ALT_INC; break;
	//case F_AILMENT: filter = E_AILMENT; break;
	//case F_SAVEABLE: filter = E_SAVEABLE; break;
	case F_EXIT: filter = E_EXIT; break;
	}
	mapEffectFilterPattern->setEnabled(true);
//	mapEffectFilterPattern->setEffectColorPair(color, filter);
}


void MapEditor::processMouseInput(int input)
{
	MEVENT event;
	nc_getmouse(&event);
	if (event.bstate & BUTTON1_CLICKED)
	{
		int pY, pX;
		map->translateCoords(event.y, event.x, pY, pX);

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
		if (mapEffectFilterPattern->isEnabled())
		{
			map->setEffect(y, x, mapEffectFilterPattern->getEffect());
		}
		else
		{
			chtype c = (chtype)drawChar;
			tileMap->setDatum(y, x, c | textColor | bkgdColor);
		}
	}
	break;

	case FILL:
		fill(y, x);
		break;
	}
	setModified(true);
}


void MapEditor::processShiftDirectionalInput(int input)
{
	hl->setHighlighting(true);
	switch (input)
	{
	case KEY_SUP: input = KEY_UP; break;
	case KEY_SDOWN: input = KEY_DOWN; break;
	case KEY_SLEFT: input = KEY_LEFT; break;
	case KEY_SRIGHT: input = KEY_RIGHT; break;
	}

	mp->processMovementInput(input);
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
		hl->setHighlighting(false);
		mp->processMovementInput(input);

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
		if (hl->isHighlighting())
			hl->erase();
		else
		{
			if (mapEffectFilterPattern->isEnabled())
			{
				map->getLayer(LAYER_EFFECT)->setDatum(curY, curX, E_NONE);
				//map->removeEffect(curY, curX, mapEffectFilterPattern->getEffect());
			}
			else
				tileMap->setDatum(curY, curX, ' ');
		}
		break;
	case CTRL_C:
		hl->copy(); 
		hl->setHighlighting(false); 
		break;
	case CTRL_V:
		hl->paste(); 
		setModified(true);
		break;
	case CTL_UP:
	case CTL_DOWN:
		hl->flip(HL_VER);
		setModified(true);
		break;
	case CTL_LEFT:
	case CTL_RIGHT:
		hl->flip(HL_HOR);
		setModified(true);
		break;

	//case CTRL_B: mp->setBounded(bounded = !bounded); break;
	case CTRL_D: mp->setViewMode(VM_DYNAMIC); break;
	case CTRL_L: mp->setViewMode(VM_LOCK); break;
	case CTRL_P: mp->setViewMode(VM_CENTER); break;

	case KEY_MOUSE:
		processMouseInput(input);
		break;
	default: //any other printable character not specified above
	 	if (input >= ' ' && input <= '~')
		{
			drawChar = input;

			if (hl->isHighlighting())
				hl->fill(drawChar | bkgdColor | textColor);
			
			applyTool(curY, curX);
		}
		
		break;
	}
	return true;
}


void MapEditor::fill(int sourceRow, int sourceCol)
{
	list<int> fillPoints;

	chtype replaceTile = tileMap->getDatum(sourceRow, sourceCol);
		//mvwinch(viewport, sourceRow, sourceCol); //make sure this isn't reversed
	chtype fillTile = drawChar | bkgdColor | textColor;
	if (drawChar == ' ')
	{
		fillTile = drawChar | bkgdColor; //we don't need the text color for blank spaces
	}
	
	int totalCols = map->getTotalCols();
	int totalRows = map->getTotalRows();

	if (replaceTile != fillTile) //meaning we have chosen the same fill character as what the cursor is currently over
		fillPoints.push_back(sourceRow * totalCols + sourceCol);

	int editTile;
	while (fillPoints.empty() == false)
	{
		editTile = fillPoints.front(); //consume head of queue
		fillPoints.pop_front();

		int y = editTile / totalCols;
		int x = editTile % totalCols;

		tileMap->setDatum(y, x, fillTile);

		//check adjacent positions to see if they have the replace character
		int rightTile = x + 1;
		if (rightTile < totalCols)//make sure it is on fillable map area
		{
			if(tileMap->getDatum(y, rightTile) == replaceTile)
			{
				fillPoints.push_back(y * totalCols + rightTile);
				tileMap->setDatum(y, rightTile, fillTile);
			}
		}

		int leftTile = x - 1;
		if (leftTile >= 0)//make sure it is on fillable map area
		{
			if (tileMap->getDatum(y, leftTile) == replaceTile)
			{
				fillPoints.push_back(y * totalCols + leftTile);
				tileMap->setDatum(y, leftTile, fillTile);
			}
		}

		int downTile = y + 1;
		if (downTile < totalRows)//make sure it is on fillable map area
		{
			if (tileMap->getDatum(downTile, x) == replaceTile)
			{
				fillPoints.push_back(downTile * totalCols + x);
				tileMap->setDatum(downTile, x, fillTile);
			}
		}
		
		int upTile = y - 1;
		if (upTile >= 0)//make sure it is on fillable map area
		{
			if (tileMap->getDatum(upTile, x) == replaceTile)
			{
				fillPoints.push_back(upTile * totalCols + x);
				tileMap->setDatum(upTile, x, fillTile);
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
	xyLbl->setText(buf);
	
	
	//mvprintw(bottom + topRulerRow + 1, sideRulerCol + 25, "H:", map->getTotalRows());
	//mvprintw(bottom + topRulerRow + 1, sideRulerCol + 35, "W:", map->getTotalCols());
	
	wnoutrefresh(stdscr);
	wnoutrefresh(frame);

	
	cm->draw();
	
	chtype cursorChar = mvwinch(viewport, curY - image->getUlY(), curX - image->getUlX());
	waddch(viewport, cursorChar | A_REVERSE);
	wnoutrefresh(viewport);
}



