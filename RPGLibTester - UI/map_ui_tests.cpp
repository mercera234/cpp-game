#include "map_ui_tests.h"
#include "Highlighter.h"
#include "MegaMap.h"
#include "MapMetadata.h"
#include "Factory.h"
#include "AutoMap.h"
#include "FreeMovementProcessor.h"
#include "SpiralBlitAnimation.h"

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


void exitMapTest()
{
	//int height = 7;
	//int width = 14;
	//resize_term(height, width);
	//WINDOW* viewport = dupwin(stdscr);

	//int mapHeight = height + (height / 2);
	//int mapWidth = width + (width / 2);
	//
	////setup map 1
	//Map* map = new Map("test", mapHeight, mapWidth, viewport);
	//Image* img = map->getDisplay(); //new Image(mapHeight, mapWidth, viewport);
	//_2DStorage<chtype>* tileMap = img->getTileMap();

	//BorderExit* eastExit = new BorderExit(B_EAST, 0, mapHeight, 2);

	//tileMap->setDatum(0, 0, '1'); //upper left
	//tileMap->setDatum(0, mapWidth - 1, '2'); //upper right
	//tileMap->setDatum(mapHeight - 1, 0, '3'); //lower left
	//tileMap->setDatum(mapHeight - 1, mapWidth - 1, '4'); //lower right

	//map->setPosition(0, 0);

	////setup map 2
	//Map* map2 = new Map("test2", mapHeight, mapWidth, viewport);
	//Image* img2 = map2->getDisplay();
	//_2DStorage<chtype>* tileMap2 = img2->getTileMap();

	//tileMap2->setDatum(0, 0, '5'); //upper left
	//tileMap2->setDatum(0, mapWidth - 1, '6'); //upper right
	//tileMap2->setDatum(mapHeight - 1, 0, '7'); //lower left
	//tileMap2->setDatum(mapHeight - 1, mapWidth - 1, '8'); //lower right

	////position cursor
	//short centerY = getmaxy(viewport) / 2;
	//short centerX = getmaxx(viewport) / 2;

	//short curY = centerY;
	//short curX = centerX;

	//Map* curMap = map;

	//bool playing = true;
	//while (playing)
	//{
	//	curMap->draw();

	//	//mvwaddch(viewport, centerY + 1, centerX + 1, '@' | 0x0f000000); //for always drawing cursor in center of screen
	//	mvwaddch(viewport, curY - curMap->getUlY(), curX - curMap->getUlX(), '@' | 0x0f000000); //for always drawing cursor in center of screen
	//	wnoutrefresh(viewport);
	//	doupdate();
	//	int input = getch();

	//	int step = 0;
	//	short* axis = 0;
	//	bool dirKeyPressed = false;
	//	switch (input)
	//	{
	//	case KEY_UP: step = -1; axis = &curY; dirKeyPressed = true; break;
	//	case KEY_DOWN: step = 1; axis = &curY; dirKeyPressed = true; break;
	//	case KEY_LEFT: step = -1; axis = &curX; dirKeyPressed = true; break;
	//	case KEY_RIGHT: step = 1; axis = &curX; dirKeyPressed = true; break;
	//	case KEY_ESC: playing = false; break;
	//	}
	//	
	//	if (dirKeyPressed)
	//	{
	//		*axis += step;
	//		axis == &curY ? curMap->shift(step, 0) : curMap->shift(0, step);
	//	}



	//}
}

void exitTest()
{
	//int height = 7;
	//int width = 14;
	//resize_term(height, width);
	//WINDOW* viewport = dupwin(stdscr);
	//BorderExit* eastExit = new BorderExit(B_EAST, 0, 3, 1);
	//BorderExit* westExit = new BorderExit(B_WEST, 0, 3, 0);
	//BorderExit* northExit = new BorderExit(B_NORTH, 5, 8, 2);
	//BorderExit* southExit = new BorderExit(B_SOUTH, 5, 8, 3);

	////position cursor
	//short centerY = getmaxy(viewport) / 2;
	//short centerX = getmaxx(viewport) / 2;

	//short curY = centerY;
	//short curX = centerX;
	//char mapId = '0';
	//bool playing = true;
	//while (playing)
	//{
	//	wclear(viewport);
	//	mvwaddch(viewport, 0, 0, mapId);
	//	mvwaddch(viewport, curY, curX, '@' | 0x0f000000); //for always drawing cursor in center of screen
	//	
	//	wnoutrefresh(viewport);
	//	doupdate();
	//	int input = getch();

	//	int step = 0;
	//	short* axis = 0;
	//	short* perpAxis = 0;
	//	bool dirKeyPressed = false;

	//	switch (input)
	//	{
	//	case KEY_UP: step = -1; axis = &curY; perpAxis = &curX; dirKeyPressed = true; break;
	//	case KEY_DOWN: step = 1; axis = &curY; perpAxis = &curX; dirKeyPressed = true; break;
	//	case KEY_LEFT: step = -1; axis = &curX; perpAxis = &curY; dirKeyPressed = true; break;
	//	case KEY_RIGHT: step = 1; axis = &curX; perpAxis = &curY; dirKeyPressed = true; break;
	//	case KEY_ESC: playing = false; break;
	//	}

	//	if (dirKeyPressed)
	//	{
	//		*axis += step;
	//		int oppSide = 0;
	//		if (curY < 0 || curY >= height || curX < 0 || curX >= width) //walked off edge
	//		{
	//			BorderExit* border = NULL;

	//			if (curY >= height)
	//			{
	//				border = southExit;
	//				oppSide = 0;
	//			}
	//			else if (curY < 0)
	//			{
	//				border = northExit;
	//				oppSide = height - 1;
	//			}
	//			else if (curX >= width)
	//			{					
	//				border = eastExit;
	//				oppSide = 0;
	//			}
	//			else if (curX < 0)
	//			{
	//				border = westExit;
	//				oppSide = width - 1;
	//			}

	//			if (*perpAxis >= border->start && *perpAxis < border->start + border->length)
	//			{
	//				mapId = border->destMapId + 48;
	//				*axis = oppSide;
	//			}


	//		}

	//	}



	//}
}

/*
Only tests viewing a megamap as a high level map
*/
void autoMapTest()
{
	MegaMap* mm = new MegaMap(newwin(10, 20, 1, 1), 64, 64);

	MapMetadata* map = new MapMetadata();
	map->hX = 1;
	map->hY = 1;
	map->layer = 0;
	map->visualId = 'a';
	map->unitsTall = 2;
	map->unitsWide = 3;

	MapMetadata* map2 = new MapMetadata();
	map2->hX = 1;
	map2->hY = 3;
	map2->layer = 0;
	map2->visualId = 'b';
	map2->unitsTall = 1;
	map2->unitsWide = 8;

	mm->addMap(map);
	mm->addMap(map2);

	bool playing = true;
	while (playing)
	{
		mm->draw();
		doupdate();

		int input = getch();


		switch (input)
		{
		case KEY_ESC: playing = false; break;
		case KEY_RIGHT: mm->shift(0, 1); break;
		case KEY_LEFT: mm->shift(0, -1); break;
		case KEY_DOWN: mm->shift(1, 0); break;
		case KEY_UP: mm->shift(-1, 0); break;
		default:
			mm->removeMap(input - 48);
			break;
		}
	}
}


void megaMapTest()
{
	MegaMap* mm = new MegaMap(newwin(10, 20, 1, 1), 64, 64);





	//MapMetadata* map = new MapMetadata();
	//map->hX = 1;
	//map->hY = 1;
	//map->layer = 0;
	//map->visualId = 'a';
	//map->unitsTall = 2;
	//map->unitsWide = 3;
	//map->mapId = 1;

	//MapMetadata* map2 = new MapMetadata();
	//map2->hX = 1;
	//map2->hY = 3;
	//map2->layer = 0;
	//map2->visualId = 'b';
	//map2->unitsTall = 1;
	//map2->unitsWide = 8;
	//map2->mapId = 2;

	//mm->addMap(map);
	//mm->addMap(map2);

	/*bool playing = true;
	while (playing)
	{
	mm->draw();
	doupdate();

	int input = getch();


	switch (input)
	{
	case KEY_ESC: playing = false; break;
	case KEY_RIGHT: mm->shift(0, 1); break;
	case KEY_LEFT: mm->shift(0, -1); break;
	case KEY_DOWN: mm->shift(1, 0); break;
	case KEY_UP: mm->shift(-1, 0); break;
	default:
	mm->removeMap(input - 48);
	break;
	}
	}*/
}

void hiLevelMapTest()
{
	Factory f;

	int unitsWide = 51;
	int unitsHigh = 23;
	resize_term(unitsHigh, unitsWide);



	MapRepository* mapRepo = new MapRepository(unitsHigh, unitsWide);


	AutoMap autoMap(stdscr, 7, 9);

	mapRepo->add(*(f.createMap(stdscr, 0, unitsHigh * 2, unitsWide * 2, '0', 0, 0)));
	mapRepo->add(*(f.createMap(stdscr, 1, unitsHigh * 2, unitsWide, '1', 1, 2)));
	mapRepo->add(*(f.createMap(stdscr, 2, unitsHigh * 5, unitsWide, '2', 2, 1)));
	mapRepo->add(*(f.createMap(stdscr, 3, unitsHigh, unitsWide * 2, '3', 6, 2)));
	mapRepo->add(*(f.createMap(stdscr, 4, unitsHigh, unitsWide, '4', 5, 3)));
	mapRepo->add(*(f.createMap(stdscr, 5, unitsHigh * 3, unitsWide * 3, '5', 2, 3)));
	mapRepo->add(*(f.createMap(stdscr, 6, unitsHigh, unitsWide * 3, '6', 3, 6)));
	mapRepo->add(*(f.createMap(stdscr, 7, unitsHigh * 2, unitsWide, '7', 1, 8)));
	mapRepo->add(*(f.createMap(stdscr, 8, unitsHigh * 2, unitsWide * 2, '8', 1, 6)));

	autoMap.setMapRepo(mapRepo);
	autoMap.setCurrMap(0);
	autoMap.updateDisplay();

	int curY = 0;
	int curX = 0;

	Image* img = autoMap.getDisplay();

	curs_set(CURSOR_INVISIBLE);
	FreeMovementProcessor mp(img, &curY, &curX);
	mp.setViewMode(ViewMode::CENTER);
	bool playing = true;

	while (playing)
	{
		autoMap.draw();

		//draw character location

		TUI::printOnBkgd('A' | COLOR_PAIR(COLOR_YELLOW_BOLD), stdscr, 0 - img->getUlY(), 0 - img->getUlX());

		wnoutrefresh(stdscr);

		doupdate();

		//process input
		int input = getch();
		switch (input)
		{
		case KEY_ESC: playing = false; break;

			//move the active map around
		case KEY_RIGHT:
		case KEY_LEFT:
		case KEY_UP:
		case KEY_DOWN:
			mp.processMovementInput(input);
			break;
		default:
		{
			int mapId = input - 48;
			autoMap.setCurrMap(mapId);
			autoMap.discover(mapId);
			autoMap.visit(mapId, 0, 1);
		}

		break;
		}
	}

}

void factoryTest()
{
	Factory f;

	resize_term(23, 51);

	Map* aMap = f.createMap(7, 23, 51, '&', stdscr);

	aMap->draw();
	mvaddch(1, 1, aMap->getId()); //displays ^G for 7
	doupdate();

	int input = getch();
}

void mapRepositoryTest()
{
	Factory f;

	int unitsWide = 51;
	int unitsHigh = 23;
	resize_term(unitsHigh, unitsWide);

	Map* aMap = f.createMap(0, unitsHigh, unitsWide, '&', stdscr);
	Map* bMap = f.createMap(1, unitsHigh * 2, unitsWide, '2', stdscr);
	Map* cMap = f.createMap(2, unitsHigh, unitsWide * 2, 'l', stdscr);

	MapRepository repo(unitsHigh, unitsWide);

	repo.add(*aMap);
	repo.add(*bMap);
	repo.add(*cMap);

	bool playing = true;

	Map* currMap = aMap;

	while (playing)
	{
		//draw map
		currMap->draw();

		//add y,x coordinates to screen
		//mvwprintw(screen, scrHeight - 2, scrWidth - 16, "y:%+4u x:%+4u", mainC->y, mainC->x);
		wnoutrefresh(stdscr);

		doupdate();

		//process input
		int input = getch();
		switch (input)
		{
		case KEY_ESC: playing = false; break;

			//move the active map around
		case KEY_RIGHT:
			currMap->getDisplay()->shift(0, 1);
			break;
		case KEY_LEFT:
			currMap->getDisplay()->shift(0, -1);
			break;
		case KEY_UP:
			currMap->getDisplay()->shift(-1, 0);
			break;
		case KEY_DOWN:
			currMap->getDisplay()->shift(1, 0);
			break;
		default:
			currMap = repo.find(input - 48);//switch the active map
			break;
		}
	}

}


void animationTest()
{
	resize_term(screenHeight, screenWidth);

	Map map0(stdscr, "data\\water_templ.map");
	Image* img = map0.getDisplay();
	img->draw(); //load drawing into window (wouldn't normally draw outside of input process update loop
	doupdate();

	getch();

	//copy image on screen to new image
	//RandomBlitAnimation anim(img);
	SpiralBlitAnimation anim(img);
	anim.setSpeed(4); //4 is optimum speed for spiral

	curs_set(CURSOR_INVISIBLE);
	bool playing = true;
	bool dir = true;

	while (playing)
	{
		//draw
		anim.draw();
		doupdate();

		//input
		if (anim.isPlaying() == false)
		{
			int input = getch();

			switch (input)
			{
			case KEY_ESC: playing = false; break;
			default:
				dir = !dir;
				anim.setPlayDirection(dir);
				anim.play();
				break;
			}


		}
		else //input comes from animation
		{
			anim.frame();
		}
	}
}