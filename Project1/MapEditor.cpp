#include "MapEditor.h"
#include "TUI.h"
#include "FileChooser.h"
#include <dirent.h>
#include <string>
#include "2DStorage.h"

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

	state = MAP_EDIT;

	modified = false; 

	keypad(stdscr, true);

	//setup filename
	fileName = DEF_FILENAME;
	WINDOW* textWin = newwin(1, 15, 0, 1);
	fileNameLbl = new TextLabel(textWin, fileName);
	
	
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
	cm->registerControl(mapEffectFilterPattern, 0, (Command*)0);
	cm->registerControl(hl, 0, (Command*)0);
	cm->registerControl(fileNameLbl, 0, (Command*) 0);
	cm->registerControl(topRuler, 0, (Command*)0);
	cm->registerControl(textTitle, 0, (Command*)0);
	cm->registerControl(bkgdTitle, 0, (Command*)0);
	cm->registerControl(toolTitle, 0, (Command*)0);
	cm->registerControl(filterTitle, 0, (Command*)0);
	cm->registerControl(xyLbl, 0, (Command*)0);
	cm->registerControl(hLbl, 0, (Command*)0);
	cm->registerControl(wLbl, 0, (Command*)0);
	
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




void MapEditor::processGlobalInput(int input)
{
	//handle preliminary input
	switch (input)
	{
	case KEY_ESC: //quit
		if (modified)
		{
			Frame* f = createConfirmDialog();

			cm->registerControl(f, KEY_LISTENER, confirmQuitCallback);
			cm->setFocus(f);
		}
		else
		{
			cm->prepareForShutdown();
		}
		break;
	case CTRL_N: 
		if (modified)
		{
			Frame* f = createConfirmDialog();
			
			cm->registerControl(f, KEY_LISTENER, confirmNewCallback);
			cm->setFocus(f);
		}
		else
		{
			newMap();
		}
		break;
	case CTRL_O:
		if (modified)
		{
			Frame* f = createConfirmDialog();

			cm->registerControl(f, KEY_LISTENER, confirmOpenCallback);
			cm->setFocus(f);
		}
		else
		{
			setupFileDialog(OPEN_DIALOG);
		}
		
		break;
	case CTRL_S: 
		if (modified) //save only if there are changes
			setupFileDialog(SAVE_DIALOG);
		break;
	}
}

Frame* MapEditor::createConfirmDialog()
{
	//create confirmation dialog
	string confirmMsg = "Are you sure? You have unsaved changes.";

	int width, height;  getmaxyx(stdscr, height, width);
	WINDOW* fWin = newwin(4, confirmMsg.length() + 2, (height - 4) / 2, (width - 40) / 2);
	WINDOW* cdWin = derwin(fWin, 1, confirmMsg.length(), 2, 1);
	Menu* cdMenu = new Menu(cdWin, 1, 2);
	cdMenu->setItem("No", "", 0, 0);
	cdMenu->setItem("Yes", "", 1, 1);

	Frame* f = new Frame(fWin, cdMenu);
	f->setText(confirmMsg, 1, 1);
	f->setModal(true);
	return f;
}


void MapEditor::newMap()
{
	map->reset();
	modified = false;
	fileName = DEF_FILENAME;
	fileNameLbl->setText(fileName);
}




void MapEditor::setupFileDialog(int dialogType)
{
	//open file dialog
	char buf[256];
	GetFullPathName(".", 256, buf, NULL);
	string fullPath(buf);

	int height = 12;
	int width = 42;
	WINDOW* main = newwin(height, width, (getmaxy(stdscr) - height) / 2, (getmaxx(stdscr) - width) / 2);
	WINDOW* w = derwin(main, height - 2, width - 2, 1, 1);

	FileChooser* fd = new FileChooser(w, fullPath, dialogType, DEF_MAP_EXTENSION);

	Frame* f = new Frame(main, fd);
	f->setModal(true);

	cm->registerControl(f, KEY_LISTENER, fileDialogCallback);
	cm->setFocus(f);
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

void MapEditor::confirmNewCallback(void* caller, void* ptr, int input) //static
{
	MapEditor* me = (MapEditor*)caller;
	me->confirmDialogDriver((Controllable*)ptr, input, NEW_MAP);
}

void MapEditor::confirmOpenCallback(void* caller, void* ptr, int input) //static
{
	MapEditor* me = (MapEditor*)caller;
	me->confirmDialogDriver((Controllable*)ptr, input, OPEN_MAP);
}

void MapEditor::confirmQuitCallback(void* caller, void* ptr, int input) //static
{
	MapEditor* me = (MapEditor*)caller;
	me->confirmDialogDriver((Controllable*)ptr, input, QUIT_MAP);
}

void MapEditor::fileDialogCallback(void* caller, void* ptr, int input) //static
{
	MapEditor* me = (MapEditor*)caller;
	me->fileDialogDriver((Controllable*)ptr, input);
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

void MapEditor::fileDialogDriver(Controllable* dialog, int input)
{
	Frame* f = (Frame*)dialog;
	FileChooser* fd = (FileChooser*) f->getControl();

	string fileChosen;
	switch (input)
	{
	case KEY_DOWN: fd->driver(REQ_DOWN_ITEM);   break;
	case KEY_UP: fd->driver(REQ_UP_ITEM); break;
	case CTRL_Q: cm->popControl(); cm->setFocus(map); break;
	case '\r':
		fileChosen = fd->driver(REQ_TOGGLE_ITEM);
		break;
	default:
		fd->driver(input);
		break;
	}

	if (fileChosen.empty() == false)
	{
		switch (fd->getType())
		{
		case OPEN_DIALOG: map->load(fileChosen); break;
		case SAVE_DIALOG: map->save(fileChosen); break;
		}
		modified = false;

		int pos = fileChosen.find_last_of('\\');
		fileName = fileChosen.substr(pos + 1, fileChosen.length());

		fileNameLbl->setText(fileName);
		cm->popControl(); 
		cm->setFocus(map);
	}
}

void MapEditor::confirmDialogDriver(Controllable* c, int input, int confirmMethod)
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
			cm->popControl();
			switch (confirmMethod)
			{
			case NEW_MAP: newMap(); cm->setFocus(map); break;
			case OPEN_MAP: setupFileDialog(OPEN_DIALOG); break;
			case QUIT_MAP: 
				cm->popControl();
				cm->prepareForShutdown(); 
				break;
			}
			
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
	markModified();
}

void MapEditor::markModified()
{
	modified = true;
	fileNameLbl->setText(fileName + "*");
}

//void MapEditor::processMovementInput(int input)
//{
//	switch (input)
//	{
//	case CTL_HOME: //use two movements to upper left corner
//		processDirectionalInput(DIR_LEFT, canvasCols);
//		processDirectionalInput(DIR_UP, canvasRows);
//		break;
//	case CTL_END: //use two movements to lower right corner
//		processDirectionalInput(DIR_RIGHT, canvasCols);
//		processDirectionalInput(DIR_DOWN, canvasRows);
//		break;
//	default:
//		int dir = getDirectionFromKey(input);
//		int magnitude = getMoveMagnitudeFromKey(input);
//		processDirectionalInput(dir, magnitude);
//		break;
//	}
//}
//
//int MapEditor::getMoveMagnitudeFromKey(int key)
//{
//	int magnitude = 0;
//	switch (key)
//	{
//	case KEY_UP:
//	case KEY_DOWN:
//	case KEY_LEFT:
//	case KEY_RIGHT: magnitude = 1;
//		break;
//	case KEY_PGUP://up down paging
//	case KEY_PGDN: magnitude = visibleRows;
//		break;
//	case CTL_PGUP://left right paging
//	case CTL_PGDN: magnitude = visibleCols;
//		break;
//
//	case KEY_HOME: 
//	case KEY_END: magnitude = canvasCols;
//		break;
//	}
//	return magnitude;
//}
//
//int MapEditor::getDirectionFromKey(int key)
//{
//	int dir;
//	switch (key)
//	{
//	case KEY_PGUP:
//	case KEY_UP: dir = DIR_UP; 
//		break;
//	case KEY_PGDN:
//	case KEY_DOWN: dir = DIR_DOWN; 
//		break;
//	case KEY_HOME:
//	case CTL_PGUP:
//	case KEY_LEFT: dir = DIR_LEFT; 
//		break;
//	case KEY_END:
//	case CTL_PGDN:
//	case KEY_RIGHT: dir = DIR_RIGHT; 
//		break;
//	default:
//		dir = DIR_ERR;
//	}
//	return dir;
//}
//
//void MapEditor::processDirectionalInput(int dirInput, int magnitude)
//{
//	short* axis = NULL;
//	int step = 1;
//
//	switch (dirInput)
//	{
//	case DIR_UP:
//		axis = &curY;
//		step = -step;
//		break;
//	case DIR_DOWN:
//		axis = &curY;
//		break;
//	case DIR_LEFT:
//		axis = &curX;
//		step = -step;
//		break;
//	case DIR_RIGHT:
//		axis = &curX;
//		break;
//	}
//
//	for (int i = 0; i < magnitude; i++) //take repeated steps until move is over
//	{
//		//move the cursor
//		*axis += step;
//
//		//verify that cursor is within bounds
//		if (curX < 0 || curY < 0 || curX >= canvasCols || curY >= canvasRows)
//		{
//			*axis -= step; break;//reverse step and quit
//		}
//		
//		//shift map
//	//	axis == &curY ? map->shift(step, 0) : map->shift(0, step);
//		axis == &curY ? image->shift(step, 0) : image->shift(0, step);
//
//		//apply brush if necessary
//		if (tool == BRUSH)
//			applyTool(curY, curX);
//	}
//}


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
	//processMovementInput(input);
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
		//processMovementInput(input);
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
		markModified();
		break;
	case CTL_UP:
	case CTL_DOWN:
		hl->flip(HL_VER);
		markModified();
		break;
	case CTL_LEFT:
	case CTL_RIGHT:
		hl->flip(HL_HOR);
		markModified();
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



