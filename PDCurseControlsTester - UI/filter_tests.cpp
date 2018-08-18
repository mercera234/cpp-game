void filterTest()
{
	//WINDOW* viewport = dupwin(stdscr);
	//
	//Map* map = new Map("test", 10, 30, viewport);

	//
	//chtype* layer = map->getDisplay()->getTileMap();

	//short centerY = getmaxy(viewport) / 2;
	//short centerX = getmaxx(viewport) / 2;
	//short y = -centerY;
	//short x = -centerX;
	//short curY = 0;
	//short curX = 0;

	//map->setPosition(y, x);

	//char asciiStart = ' ';
	//char asciiEnd = '~';
	//char asciiPtr = asciiStart;
	//int totalTiles = 10 * 30;
	//for (int i = 0; i < totalTiles; i++)
	//{
	//	layer[i] = (chtype)asciiPtr++ | COLOR_PAIR(COLOR_YELLOW);

	//	if (asciiPtr >= asciiEnd)
	//		asciiPtr = asciiStart;
	//}

	//FilterPattern* f = new BWFilter(map);

	//bool playing = true;
	//bool filterStatus = false;
	//while (playing)
	//{
	//	wclear(viewport);
	//	wbkgd(viewport, '%');
	//	wnoutrefresh(viewport);
	//	f->draw();
	//	
	//	mvwaddch(viewport, centerY, centerX, '@' | 0x0f000000);
	//	wnoutrefresh(viewport);
	//	doupdate();
	//	int input = getch();


	//	switch (input)
	//	{
	//	case KEY_UP: y--; curY--; break;
	//	case KEY_DOWN: y++; curY++; break;
	//	case KEY_LEFT: x--; curX--; break;
	//	case KEY_RIGHT: x++; curX++; break;
	//	case KEY_ESC: playing = false; break;
	//	default: //fill with printable character
	//		//toggle filter
	//		f->setEnabled(filterStatus = !filterStatus);
	//		break;
	//	}

	//	map->setPosition(y, x);
	//}


}