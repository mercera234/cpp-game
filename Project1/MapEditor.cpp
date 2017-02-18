#include "MapEditor.h"
#include "TUI.h"
#include "FileChooser.h"
#include <dirent.h>
//#include "MouseHelper.h"

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
	fileNameLbl = new TextLabel(textWin, fileName);
	
	
	map = new Map(fileName, canvasRows, canvasCols, viewport);

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
	cm->registerControl(fileNameLbl, 0, 0);
	cm->registerControl(topRuler, 0, 0);
	cm->registerControl(textTitle, 0, 0);
	cm->registerControl(bkgdTitle, 0, 0);
	cm->registerControl(toolTitle, 0, 0);
	cm->registerControl(filterTitle, 0, 0);
	cm->registerControl(xyLbl, 0, 0);
	cm->registerControl(hLbl, 0, 0);
	cm->registerControl(wLbl, 0, 0);
	
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
	toolPalette = new Palette(1, 3, topRulerRow + 15, paletteLeftEdge);
	toolPalette->setItem("Dot", '.', 0, 0);
	toolPalette->setItem("Fill", 'F', 0, 1);
	toolPalette->setItem("Brush", 'B', 0, 2);
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
		if (modified == false) break; //don't save when there aren't any changes
		{
			setupFileDialog(SAVE_DIALOG);
		}
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
	case BRUSH:
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
	fileNameLbl->setText(fileName + "*");
}


void MapEditor::processDirectionalInput(int input)
{
	int* axis = NULL;
	int border;
	int magnitude;
	int leftSide;
	int rightSide;

	switch (input)
	{
	case KEY_UP:
		axis = &y;
		border = -1;
		magnitude = -1;
		leftSide = *axis - 1;
		rightSide = border;
		break;
	case KEY_DOWN:
		axis = &y;
		border = canvasRows;
		magnitude = 1;
		leftSide = border;
		rightSide = *axis + 1;
		break;
	case KEY_LEFT:
		axis = &x;
		border = -1;
		magnitude = -1;
		leftSide = *axis - 1;
		rightSide = border;
		break;
	case KEY_RIGHT:
		axis = &x;
		border = canvasCols;
		magnitude = 1;
		leftSide = border;
		rightSide = *axis + 1;
		break;
	}

	if (leftSide > rightSide)
	{
		*axis += magnitude;
		axis == &y ? map->shiftVert(magnitude) : map->shiftHor(magnitude);
		
		if (tool == BRUSH)
			applyTool(y, x);
	}	
}


bool MapEditor::processMapInput(int input)
{
	switch (input)
	{
	case KEY_UP:
	case KEY_DOWN:
	case KEY_LEFT:
	case KEY_RIGHT: processDirectionalInput(input);
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
	
	char buf[20];
	sprintf_s(buf, "x: %+4d  y: %+4d", x, y);
	xyLbl->setText(buf);
	
	
	//mvprintw(bottom + topRulerRow + 1, sideRulerCol + 25, "H:", map->getTotalRows());
	//mvprintw(bottom + topRulerRow + 1, sideRulerCol + 35, "W:", map->getTotalCols());
	
	wnoutrefresh(stdscr);
	wnoutrefresh(frame);

	cm->draw();


	//add filter to map
	//for (int row = 0; row < visibleRows; row++)
	//{
	//	for (int col = 0; col < visibleCols; col++)
	//	{


	//		chtype c = mvwinch(viewport, row, col); //colorless filter

	//		//get color values
	//		int textC = c & 0x0f000000;
	//		int bkgdC = c & 0xf0000000;

	//		//translate to filters
	//		int fTextC;
	//		int fBkgdC;

	//		if (bkgdC == 0x00000000)
	//		{
	//			fBkgdC = bkgdC;
	//			fTextC = 0x00000000; //black text -> goes to white with black bkgd!
	//		}
	//		else if ((bkgdC & 0x80000000) != 0) //any bold color
	//		{
	//			fBkgdC = 0xf0000000; //white bkgd
	//			fTextC = 0x00000000; //black text
	//		}
	//		else //pair with regular bkgd color
	//		{
	//			fBkgdC = 0x80000000;//set to gray bkgd
	//			fTextC = 0x00000000; //black text
	//		}

	//		c &= 0x0000ffff; //remove all color
	//		waddch(viewport, c | fTextC | fBkgdC); //reapply filtered colors
	//	}
	//}


	chtype cursorChar = mvwinch(viewport, centerY, centerX);
	waddch(viewport, cursorChar | A_REVERSE);

	wnoutrefresh(viewport);
}



