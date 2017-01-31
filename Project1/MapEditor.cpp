#include "MapEditor.h"
#include "TUI.h"
#include "MouseHelper.h"

MapEditor::MapEditor()
{
	canvasRows = 64;
	canvasCols = 64;

	topRulerRow = 2;
	sideRulerCol = 19;

	visibleRows = 23;
	visibleCols = 51;

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

	unsavedChanges = false;

	keypad(stdscr, true);

	//setup filename
	WINDOW* textWin = newwin(1, 15, 0, 1);
	mapName = new TextField(textWin);
	fileName = "Untitled.map";
	mapName->setText(fileName);

	map = new Map(fileName, canvasRows, canvasCols, viewport);
}

void MapEditor::setupPalettes()
{
	//setup color palettes
	paletteLeftEdge = sideRulerCol + 1 + visibleCols + 5;

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

	//setup filter palette
	filterPalette = new Palette(2, 4, topRulerRow + 15, paletteLeftEdge);

	filterPalette->setItem("Obstr", 'O', 0, 0);
	filterPalette->setItem("Jumpable", 'J', 0, 1);
	filterPalette->setItem("Const Dmg", 'd', 0, 2);
	filterPalette->setItem("Inc Dmg", 'D', 0, 3);
	filterPalette->setItem("Ailment", 'A', 1, 0);
	filterPalette->setItem("Save", 'S', 1, 1);
	filterPalette->setItem("Exit", 'E', 1, 2);
}

bool MapEditor::processInput(int input)
{
	//handle preliminary input
	switch (input)
	{
	case KEY_MOUSE:
		processMouseInput(input);
		return true;
	case CTRL_Q: //use to quit, will add more logic eventually to prevent user from quitting without saving
		return false;
	case CTRL_N: 
		map->reset();
		return true; //eventually will allow us to make new map
	case CTRL_O:
		map->load("Untitled.map");
		return true;
	case CTRL_S: 
		map->save();
		return true; //eventually for saving
	}

	//process state specific input
	bool retval = false;
	switch (state)
	{
	case MAP_EDIT: retval = processMapInput(input); break;
	case MAP_NAME: 
		if (input == '\r' || input == '\n' || input == '\t')
		{
			state = MAP_EDIT;//finish editing field
		}
		retval = mapName->inputChar(input); break;
	}

	return true;
}

void MapEditor::processMouseInput(int input)
{
	MEVENT event;
	nc_getmouse(&event);
	if (event.bstate == BUTTON1_CLICKED)
	{
		if (MouseHelper::isInWindow(textPalette->getWindow(), event.y, event.x))
		{
			int colorY, colorX;
			MouseHelper::translateCoords(textPalette->getWindow(), event.y, event.x, colorY, colorX);

			PaletteItem* item = textPalette->pickItem(colorY, colorX);
			textColor = item->index; //it might make more sense to use the icon, but this is backwards compatible with what I had
		}
		else if (MouseHelper::isInWindow(bkgdPalette->getWindow(), event.y, event.x))
		{
			int colorY, colorX;
			MouseHelper::translateCoords(bkgdPalette->getWindow(), event.y, event.x, colorY, colorX);

			PaletteItem* item = bkgdPalette->pickItem(colorY, colorX);
			bkgdColor = item->index;
		}
		else if (MouseHelper::isInWindow(filterPalette->getWindow(), event.y, event.x))
		{
			int pY, pX;
			MouseHelper::translateCoords(filterPalette->getWindow(), event.y, event.x, pY, pX);

			PaletteItem* item = filterPalette->pickItem(pY, pX);
			filter = item->index;
		}
		else if (MouseHelper::isInWindow(mapName->getWindow(), event.y, event.x))
		{
			mapName->setFocus();
			state = MAP_NAME;
		}
	}
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
	default: //any other printable character not specified above
		chtype c = (chtype)input;
		c |= ((textColor << 24) & A_COLOR); 
		c |= ((bkgdColor << 28) & A_COLOR);
		map->setDisplayChar(y, x, c);
		break;
	}
	return true;
}


void MapEditor::loadMap()
{
	
}

void MapEditor::draw()
{
	mapName->draw();

	mvaddch(topRulerRow, sideRulerCol, '+');
	drawRulers();

	box(frame, 0, 0);
	mvaddstr(topRulerRow, paletteLeftEdge, "TEXT");
	mvaddstr(topRulerRow + 7, paletteLeftEdge, "BKGD");
	mvaddstr(topRulerRow + 14, paletteLeftEdge, "FILTER");

	//draw the x y coordinates
	int bottom = getmaxy(frame);
	mvprintw(bottom + topRulerRow + 1, sideRulerCol + 1, "x: %+4d  y: %+4d", x, y); //%+4d always render sign, 4 char field, int

	map->draw();

	/*The center cursor is done by retrieving the center character and then redrawing it with the A_REVERSE attr on*/
	wattron(viewport, A_REVERSE);
	char cursorChar = mvwinch(viewport, centerY, centerX);
	waddch(viewport, cursorChar);
	wattroff(viewport, A_REVERSE);

	wnoutrefresh(stdscr);
	wnoutrefresh(frame);
	
	textPalette->draw();
	bkgdPalette->draw();
	filterPalette->draw();
	
	if(state == MAP_NAME)
		mapName->setFocus();
}


void MapEditor::drawRulers()
{
	//draw top ruler
	for (int i = 0; i <= visibleCols; i++)
	{
		int digit = i % 10;
		mvprintw(topRulerRow, sideRulerCol + 1 + i, "%d", digit);
	}

	//draw side ruler
	for (int i = 0; i <= visibleRows; i++)
	{
		int digit = i % 10;
		mvprintw(topRulerRow + 1 + i, sideRulerCol, "%d", digit);
	}
}


