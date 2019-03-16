#include "map_ui_tests.h"
#include "Highlighter.h"
#include "MegaMap.h"
#include "MapMetadata.h"
#include "Factory.h"
#include "AutoMap.h"
#include "FreeMovementProcessor.h"
#include "SpiralBlitAnimation.h"
#include "Actor.h"
#include "actor_helper.h"
#include "Sprite.h"
#include "GameItem.h"

void mapEffectFilterTest()
{
	WINDOW* viewport = dupwin(stdscr);

	MapRoom map("test", 10, 30, viewport);

	ITwoDStorage<chtype>& dLayer = map.getDisplay().getTileMap();
	ITwoDStorage<EffectType>& eLayer = map.getEffectsLayer();

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
			

		dLayer.setDatum(i, tile | color);

		
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
	MapRoom* map = new MapRoom("test", 10, 30, viewport);
	Image& img = map->getDisplay();

	ITwoDStorage<chtype>& tileMap = img.getTileMap();
	
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
		tileMap.setDatum(i, (chtype)asciiPtr++);

		if (asciiPtr >= asciiEnd)
			asciiPtr = asciiStart;
	}


	Highlighter* hl = new Highlighter(&img, &curY, &curX);


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

		img.setPosition(y, x);
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

//void megaMapTest()
//{
//	MegaMap mm(newwin(10, 20, 1, 1), 64, 64);
//
//	MapMetadata map;
//	map.hX = 1;
//	map.hY = 1;
//	map.layer = 0;
//	map.visualId = 'a';
//	map.unitsTall = 2;
//	map.unitsWide = 3;
//	map.mapId = 1;
//
//	MapMetadata map2;
//	map2.hX = 1;
//	map2.hY = 3;
//	map2.layer = 0;
//	map2.visualId = 'b';
//	map2.unitsTall = 1;
//	map2.unitsWide = 8;
//	map2.mapId = 2;
//
//	mm.addMap(&map);
//	mm.addMap(&map2);
//
//	bool playing = true;
//	while (playing)
//	{
//	mm.draw();
//	doupdate();
//
//	int input = getch();
//
//
//		switch (input)
//		{
//		case KEY_ESC: playing = false; break;
//		case KEY_RIGHT: mm.shift(0, 1); break;
//		case KEY_LEFT: mm.shift(0, -1); break;
//		case KEY_DOWN: mm.shift(1, 0); break;
//		case KEY_UP: mm.shift(-1, 0); break;
//		default:
//			mm.removeMap(input - 48);
//			break;
//		}
//	}
//}

//void autoMapTest()
//{
//	Factory f;
//
//	int unitsWide = 51;
//	int unitsHigh = 23;
//	resize_term(unitsHigh, unitsWide);
//
//
//
//	MapRepository mapRepo(unitsHigh, unitsWide);
//
//
//	AutoMap autoMap(stdscr, 7, 9);
//
//	mapRepo.add(*(f.createMap(stdscr, 0, unitsHigh * 2, unitsWide * 2, '0', 0, 0)));
//	mapRepo.add(*(f.createMap(stdscr, 1, unitsHigh * 2, unitsWide, '1', 1, 2)));
//	mapRepo.add(*(f.createMap(stdscr, 2, unitsHigh * 5, unitsWide, '2', 2, 1)));
//	mapRepo.add(*(f.createMap(stdscr, 3, unitsHigh, unitsWide * 2, '3', 6, 2)));
//	mapRepo.add(*(f.createMap(stdscr, 4, unitsHigh, unitsWide, '4', 5, 3)));
//	mapRepo.add(*(f.createMap(stdscr, 5, unitsHigh * 3, unitsWide * 3, '5', 2, 3)));
//	mapRepo.add(*(f.createMap(stdscr, 6, unitsHigh, unitsWide * 3, '6', 3, 6)));
//	mapRepo.add(*(f.createMap(stdscr, 7, unitsHigh * 2, unitsWide, '7', 1, 8)));
//	mapRepo.add(*(f.createMap(stdscr, 8, unitsHigh * 2, unitsWide * 2, '8', 1, 6)));
//
//	autoMap.setMapRepo(&mapRepo);
//	autoMap.setCurrMap(0);
//	autoMap.updateDisplay();
//
//	int curY = 0;
//	int curX = 0;
//
//	Image* img = autoMap.getDisplay();
//
//	
//	FreeMovementProcessor mp(img, &curY, &curX);
//	mp.setViewMode(ViewMode::CENTER);
//	bool playing = true;
//
//	while (playing)
//	{
//		autoMap.draw();
//
//		//draw character location
//
//		TUI::printOnBkgd('A' | COLOR_PAIR(COLOR_YELLOW_BOLD), stdscr, 0 - img->getUlY(), 0 - img->getUlX());
//
//		wnoutrefresh(stdscr);
//
//		doupdate();
//
//		//process input
//		int input = getch();
//		switch (input)
//		{
//		case KEY_ESC: playing = false; break;
//
//			//move the active map around
//		case KEY_RIGHT:
//		case KEY_LEFT:
//		case KEY_UP:
//		case KEY_DOWN:
//			mp.processMovementInput(input);
//			break;
//		default:
//		{
//			int mapId = input - 48;
//			autoMap.setCurrMap(mapId);
//			autoMap.discover(mapId);
//			autoMap.visit(mapId, 0, 0);
//		}
//
//		break;
//		}
//	}
//
//}

void drawMegaMapTest()
{
	int unitsWide = 51;
	int unitsHigh = 23;
	resize_term(unitsHigh, unitsWide);
	
	MegaMap mm;
	mm.setDimensions(4, 4, 2);
	mm.setUnitHeight(unitsHigh);
	mm.setUnitWidth(unitsWide);
	mm.setGroundFloor(1);
	mm.setFloor("1F");
	

	Image img;
	ITwoDStorage<chtype>& tileMap = img.getTileMap();
	tileMap.fill(-1);

	img.setDimensions(4, 4);
	img.setTile(1, 1, 17);
	img.setTile(1, 2, 17);
	img.setTile(2, 1, 17);
	img.setTile(2, 2, 17);

	Image img2;
	ITwoDStorage<chtype>& tileMap2 = img2.getTileMap();
	tileMap2.fill(-1);

	img2.setDimensions(4, 4);
	img2.setTile(0, 0, 16);
	img2.setTile(0, 1, 16);
	img2.setTile(0, 2, 16);
	img2.setTile(0, 3, 16);
	

	mm.setLayerImage(0, img);
	mm.setLayerImage(1, img2);

	
	
	Pos cursor(0, 0);
	mm.setCursor(&cursor.y, &cursor.x);
	mm.visitArea();

	cursor.x = 51;
	cursor.y = 23;
	mm.setFloor("B1");
	mm.visitArea();

	AutoMap autoMap;
	autoMap.setCurrMap(&mm);
	autoMap.setWindow(stdscr);
		
	Pos unitPos = mm.getUnitPos();
	FreeMovementProcessor mp(&autoMap, &unitPos.y, &unitPos.x);
	mp.setViewMode(ViewMode::CENTER);
	bool playing = true;


	
	while (playing)
	{
		autoMap.draw();
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
			processInput(autoMap, input);
			//mp.processMovementInput(input);
			break;
		}
	}
}

void highLevelMapTest()
{
	Image highLevelMap;
	highLevelMap.setDimensions(2, 2);
	highLevelMap.setTile(0, 0, 2);
	highLevelMap.setTile(0, 1, 2);
	highLevelMap.setTile(1, 0, 1);
	highLevelMap.setTile(1, 1, 0);
	/*
	Arrangement of tiles is like
	22
	10
	*/


	WINDOW* win = newwin(2, 2, 1, 1);
	highLevelMap.setWindow(win);

	wbkgd(win, COLOR_RED << BKGDCOLOR_OFFSET);

	highLevelMap.draw();
	doupdate();
	getch();
}


void megaMapTest()
{
	MegaMap mm;
	mm.setDimensions(6, 6, 6);
	mm.setUnitHeight(23);
	mm.setUnitWidth(51);
	mm.setFloorIndex(0);

	Image img[6];
	ITwoDStorage<chtype>& tileMap = img[0].getTileMap();
	tileMap.fill(INT_MAX);

	img[0].setDimensions(6, 6);
	img[1].setDimensions(6, 6);
	img[2].setDimensions(6, 6);
	img[3].setDimensions(6, 6);
	img[4].setDimensions(6, 6);
	img[5].setDimensions(6, 6);

	img[0].setTile(1, 2, 1);
	img[0].setTile(1, 3, 1);
	img[0].setTile(1, 4, 1);
	img[0].setTile(2, 2, 1);
	img[0].setTile(2, 3, 1);
	img[0].setTile(2, 4, 1);

	img[0].setTile(1, 1, 2);
	img[0].setTile(2, 1, 2);
	img[0].setTile(3, 1, 2);
	img[0].setTile(4, 1, 2);
	
	img[1].getTileMap().fill(INT_MAX);
	img[2].getTileMap().fill(INT_MAX);
	img[3].getTileMap().fill(INT_MAX);
	img[4].getTileMap().fill(INT_MAX);
	img[5].getTileMap().fill(INT_MAX);

	mm.setLayerImage(0, img[0]);
	mm.setLayerImage(1, img[1]);
	mm.setLayerImage(2, img[2]);
	mm.setLayerImage(3, img[3]);
	mm.setLayerImage(4, img[4]);
	mm.setLayerImage(5, img[5]);


	Actor actor;
	initDefaultActor(actor);
	
	Pos actorPos(30, 120);

	mm.setCursor(&actorPos.y, &actorPos.x);

	bool playing = true;
	while (playing)
	{
		int c = getch();

		switch (c)
		{
		case KEY_DOWN: actorPos.y++; break;
		case KEY_UP: actorPos.y--; break;
		case KEY_LEFT: actorPos.x--; break;
		case KEY_RIGHT: actorPos.x++; break;
		case '+': mm.changeLayer(1); break;
		case '-': mm.changeLayer(-1); break;
		case KEY_ESC: playing = false; continue; 
		}
		
		
		Pos imc = mm.getMapRoomPos();
		Pos map = mm.getUnitPos();
		int mapId = img[0].getTile(map.y, map.x);
		if (mapId == INT_MAX)
			mapId = '!';
		std::cout << "z: " << mm.getFloorLabel() << " y: " << actorPos.y << " x: " << actorPos.x  //print actor coordinates
			<< "  mapY: " << imc.y << " mapX: " << imc.x  //print inner map coordinates
			<< "  Y: " << map.y << " X: " << map.x << ' ' << ((mapId == INT_MAX) ? '!' : mapId) << std::endl; //print high level map coordinates
	}
}


void factoryTest()
{
	Factory f;

	resize_term(23, 51);

	MapRoom* aMap = f.createMap(7, 23, 51, '&', stdscr);

	aMap->draw();
	mvaddch(1, 1, aMap->id); //displays ^G for 7
	doupdate();

	int input = getch();
}




void animationTest()
{
	resize_term(23, 51);

	
	MapRoom map0;// , "data\\water_templ.map");
	map0.setWindow(stdscr);
	FilePath path("data", &map0);
	path.load("water_templ.map");

	Image& img = map0.getDisplay();
	img.draw(); //load drawing into window (wouldn't normally draw outside of input process update loop
	doupdate();

	getch();

	//copy image on screen to new image
	//RandomBlitAnimation anim(img);
	SpiralBlitAnimation anim(&img);
	anim.setSpeed(4); //4 is optimum speed for spiral

	
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

















