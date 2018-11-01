#include "movement_tests.h"

void freeMovementProcessorTest()
{
	int height = 5;
	int width = 7;
	resize_term(height, width);
	WINDOW* viewport = dupwin(stdscr);

	int mapHeight = 9; //height + (height / 2);
	int mapWidth = 11;//width + (width / 2);

	Image* map = new Image(mapHeight, mapWidth, viewport);
	ITwoDStorage<chtype>& tileMap = map->getTileMap();

	short centerY = getmaxy(viewport) / 2;
	short centerX = getmaxx(viewport) / 2;
	short y = -centerY;
	short x = -centerX;
	int curY = 7;
	int curX = 7;

	tileMap.setDatum(0, 0, '1'); //upper left
	tileMap.setDatum(0, mapWidth - 1, '2'); //upper right
	tileMap.setDatum(mapHeight - 1, 0, '3'); //lower left
	tileMap.setDatum(mapHeight - 1, mapWidth - 1, '4'); //lower right

	FreeMovementProcessor* mp = new FreeMovementProcessor(map, &curY, &curX);


	bool playing = true;
	bool bounded = true;

	while (playing)
	{
		map->draw();

		//mvwaddch(viewport, centerY + 1, centerX + 1, '@' | 0x0f000000); //for always drawing cursor in center of screen
		mvwaddch(viewport, curY - map->getUlY(), curX - map->getUlX(), '@' | 0x0f000000); //for always drawing cursor in center of screen
		wnoutrefresh(viewport);
		doupdate();
		int input = getch();




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
		case KEY_SHOME://all the way up
		case KEY_SEND://all the way down
			mp->processMovementInput(input); break;
		case KEY_ESC: playing = false; break;

		case 'b': mp->setBounded(bounded = !bounded); break;
		case 'd': mp->setViewMode(ViewMode::DYNAMIC); break;
		case 'l': mp->setViewMode(ViewMode::LOCK); break;
		case 'c': mp->setViewMode(ViewMode::CENTER); break;
		}


	}

}