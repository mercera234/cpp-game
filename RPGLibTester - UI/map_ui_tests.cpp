#include "map_ui_tests.h"
#include "Highlighter.h"

void mapEffectFilterTest()
{
	WINDOW* viewport = dupwin(stdscr);

	Map map("test", 10, 30, viewport);

	TwoDStorage<chtype>* dLayer = map.getDisplay()->getTileMap();
	TwoDStorage<EffectType>& eLayer = map.getEffectsLayer();

	short centerY = getmaxy(viewport) / 2;
	short centerX = getmaxx(viewport) / 2;
	short y = -centerY + 1;
	short x = -centerX + 1;
	short curY = 1;
	short curX = 1;

	map.setPosition(y, x);

	char asciiStart = '!';
	char asciiEnd = '~';
	char asciiPtr = asciiStart;
	int totalTiles = 10 * 30;
	for (int i = 0; i < totalTiles; i++)
	{
		int y = i / map.getTotalCols();
		int x = i % map.getTotalCols();

		//set tile pattern
		chtype tile = ' ';

		//give frame obstructed effect
		if (y == 0 || x == 0 || y == map.getTotalRows() - 1 || x == map.getTotalCols() - 1)
		{
			tile = asciiPtr++;
			eLayer.setDatum(i, EffectType::OBSTR);
		}

		if (asciiPtr >= asciiEnd)
			asciiPtr = asciiStart;

		//set tile color
		chtype color;

		if (x < 10)
			color = setTextColor(COLOR_YELLOW); //yellow text
		else if (x >= 10 && x < 20)
			color = setBkgdColor(COLOR_YELLOW); //yellow background
		else if (x < 30)
			color = setBkgdColor(COLOR_YELLOW_BOLD); //bright yellow background

		//make one row and col jumpable
		if (y == 4 || x == 15)
		{
			color = setBkgdColor(COLOR_BLUE);
			eLayer.setDatum(i, EffectType::JUMPABLE);
		}
			

		dLayer->setDatum(i, tile | color);

		
	}

	MapEffectFilterPattern* f = new MapEffectFilterPattern(&map);

	bool playing = true;
	bool filterStatus = false;
	while (playing)
	{
		wclear(viewport);
		wbkgd(viewport, ' ');
		wnoutrefresh(viewport);
		f->draw();

		mvwaddch(viewport, centerY, centerX, '@' | 0x0f000000);
		wnoutrefresh(viewport);
		doupdate();
		int input = getch();


		switch (input)
		{
		case KEY_UP: y--; curY--; break;
		case KEY_DOWN: y++; curY++; break;
		case KEY_LEFT: x--; curX--; break;
		case KEY_RIGHT: x++; curX++;  break;
		default:
			switch (input)
			{
			case KEY_ESC: playing = false; break;
			default: //fill with printable character
					 //toggle filter
				f->setEnabled(filterStatus = !filterStatus);
				break;
			}
			continue;
		}

		//movement occurred to get here
		//check the eLayer to see if the step can be taken
		EffectType e = eLayer.getDatum(curY, curX);

		switch (e)
		{
		case EffectType::OBSTR:
			switch (input) //reverse movement
			{
			case KEY_UP: y++; curY++; break;
			case KEY_DOWN: y--; curY--; break;
			case KEY_LEFT: x++; curX++; break;
			case KEY_RIGHT: x--; curX--; break;
			}
			break;
		case EffectType::JUMPABLE:
			switch (input) //double movement
			{
			case KEY_UP: y--; curY--; break;
			case KEY_DOWN: y++; curY++; break;
			case KEY_LEFT: x--; curX--; break;
			case KEY_RIGHT: x++; curX++;  break;
			}
			break;

		}

		map.setPosition(y, x);
	}


}



void mapHighlighterTest()
{
	WINDOW* viewport = dupwin(stdscr);
	Map* map = new Map("test", 10, 30, viewport);
	Image* img = map->getDisplay();

	TwoDStorage<chtype>* tileMap = img->getTileMap();
	
	short centerY = getmaxy(viewport) / 2;
	short centerX = getmaxx(viewport) / 2;
	short y = -centerY;
	short x = -centerX;
	int curY = 0;
	int curX = 0;

	map->setPosition(y, x);

	char asciiStart = ' ';
	char asciiEnd = '~';
	char asciiPtr = asciiStart;
	int totalTiles = 10 * 30;
	for (int i = 0; i < totalTiles; i++)
	{
		tileMap->setDatum(i, (chtype)asciiPtr++);

		if (asciiPtr >= asciiEnd)
			asciiPtr = asciiStart;
	}


	Highlighter* hl = new Highlighter(img, &curY, &curX);


	bool playing = true;
	while (playing)
	{
		wclear(viewport);
		wbkgd(viewport, '%');
		wnoutrefresh(viewport);
		map->draw();
		hl->draw();
		mvwaddch(viewport, centerY, centerX, '@' | 0x0f000000);
		wnoutrefresh(viewport);
		doupdate();
		int input = getch();


		switch (input)
		{
		case KEY_UP: y--; curY--; hl->setHighlighting(false); break;
		case KEY_DOWN: y++; curY++; hl->setHighlighting(false); break;
		case KEY_LEFT: x--; curX--; hl->setHighlighting(false); break;
		case KEY_RIGHT: x++; curX++; hl->setHighlighting(false); break;
		case KEY_SLEFT: hl->setHighlighting(true); x--; curX--; break;
		case KEY_SRIGHT: hl->setHighlighting(true); x++; curX++; break;
		case KEY_SUP: hl->setHighlighting(true); y--; curY--; break;
		case KEY_SDOWN: hl->setHighlighting(true); y++; curY++; break;
		case KEY_ESC: playing = false; break;
		case CTRL_C:
			hl->copy();
			hl->setHighlighting(false);
			break;
		case CTRL_V:
			hl->paste();
			break;
		case KEY_DC: //delete highlit region			
			hl->erase();
			break;
		case CTL_UP:
		case CTL_DOWN:
			hl->flip(AXIS_VER);
			break;
		case CTL_LEFT:
		case CTL_RIGHT:
			hl->flip(AXIS_HOR);
			break;
		default: //fill with printable character
			hl->fill(input);
			break;
		}

		img->setPosition(y, x);
	}


}

void simpleMapTest()
{
	WINDOW* viewport = dupwin(stdscr);

	int height = 10;
	int width = 30;
	Map* map = new Map("test", height, width, viewport);

	Image* display = map->getDisplay();

	short centerY = getmaxy(viewport) / 2;
	short centerX = getmaxx(viewport) / 2;
	short y = -centerY;
	short x = -centerX;
	short curY = 1;
	short curX = 1;

	display->setPosition(y, x);

	char asciiStart = ' ';
	char asciiEnd = '~';
	char asciiPtr = asciiStart;
	int totalTiles = height * width;

	TwoDStorage<chtype>* data = display->getTileMap();

	for (int i = 0; i < totalTiles; i++)
	{
		int y = i / width;
		int x = i % width;

		chtype c;
		if (x < 10)
			c = (chtype)asciiPtr++ | COLOR_PAIR(COLOR_YELLOW);
		else if (x >= 10 && x < 20)
			c = (chtype)asciiPtr++ | (COLOR_YELLOW << 28); //yellow background
		else if (x < 30)
			c = (chtype)asciiPtr++ | (COLOR_YELLOW_BOLD << 28); //yellow background

		data->setDatum(i, c);

		if (asciiPtr >= asciiEnd)
			asciiPtr = asciiStart;

	}

	bool playing = true;
	bool filterStatus = false;
	while (playing)
	{
		wclear(viewport);
		wbkgd(viewport, '%');
		wnoutrefresh(viewport);

		map->draw();
		mvwaddch(viewport, centerY + 1, centerX + 1, '@' | 0x0f000000);
		wnoutrefresh(viewport);
		doupdate();
		int input = getch();


		switch (input)
		{
		case KEY_UP: y--; curY--;
			break;
		case KEY_DOWN: y++; curY++; break;
		case KEY_LEFT: x--; curX--; break;
		case KEY_RIGHT: x++; curX++; break;
		case KEY_ESC: playing = false; break;
		default: //fill with printable character
				 //toggle filter
			break;
		}

		map->setPosition(y, x);
	}
}


void realMapTest()
{
	int rows = 8;
	int cols = 8;

	WINDOW* frame = newwin(rows + 2, cols + 2, 1, 1);
	WINDOW* win = derwin(frame, rows, cols, 1, 1);

	Map m("A map", rows, cols, win);

	int y = 0;
	int x = 0;
	m.setPosition(y, x);

	bool playing = true;
	Image* img = m.getDisplay();

	while (playing)
	{
		box(frame, 0, 0);
		wnoutrefresh(frame);
		m.draw();
		doupdate();
		int c = getch();

		switch (c)
		{
		case KEY_UP: y--;  break;
		case KEY_DOWN: y++; break;
		case KEY_LEFT: x--; break;
		case KEY_RIGHT: x++; break;
		default: playing = false;
		}

		m.setPosition(y, x);
	}


}
