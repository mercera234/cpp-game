#include "map_ui_tests.h"

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