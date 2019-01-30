#include "mapRoom_tests.h"
#include "curses.h"
#include "MapRoom.h"
#include "GameItem.h"
#include "actor_helper.h"

void simpleMapTest()
{
	WINDOW* viewport = dupwin(stdscr);

	int height = 10;
	int width = 30;
	MapRoom* map = new MapRoom("test", height, width, viewport);

	Image& display = map->getDisplay();

	short centerY = getmaxy(viewport) / 2;
	short centerX = getmaxx(viewport) / 2;
	short y = -centerY;
	short x = -centerX;
	short curY = 1;
	short curX = 1;

	display.setPosition(y, x);

	char asciiStart = ' ';
	char asciiEnd = '~';
	char asciiPtr = asciiStart;
	int totalTiles = height * width;

	ITwoDStorage<chtype>& data = display.getTileMap();

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

		data.setDatum(i, c);

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
	WINDOW* win = newwin(rows, cols, 2, 2);

	MapRoom m("A map", rows, cols, win);

	int y = 0;
	int x = 0;
	m.setPosition(y, x);

	bool playing = true;
	Image& img = m.getDisplay();
	img.getTileMap().fill(' ');

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

void spriteOnImageTest()
{
	int screenHeight = 23;
	int screenWidth = 51;
	resize_term(screenHeight, screenWidth);

	Image img;
	WINDOW* screen = dupwin(stdscr);
	img.setWindow(screen);
	img.setDimensions(screenHeight, screenWidth);

	ITwoDStorage<chtype>& tileMap = img.getTileMap();
	tileMap.fill('.');

	Actor actor;
	actor.symbol = '@' | COLOR_YELLOW_BOLD << TEXTCOLOR_OFFSET;
	Pos cPos(screenHeight / 2, screenWidth / 2);
	Sprite control;
	control.pos = cPos;
	control.thing = &actor;

	GameItem item;
	item.name = "Potion";
	item.cost = 3;
	item.symbol = defaultItemSymbol;

	GameItem money;
	money.name = "Gold$";
	money.cost = 50;
	money.symbol = '$' | COLOR_YELLOW_BOLD << TEXTCOLOR_OFFSET;

	std::list<Sprite*> things;

	Sprite thing1;
	thing1.thing = &item;
	thing1.pos.y = 3;
	thing1.pos.x = 3;

	Sprite thing2;
	thing2.thing = &money;
	thing2.pos.y = 20;
	thing2.pos.x = 20;

	things.push_back(&thing1);
	things.push_back(&thing2);

	bool playing = true;
	while (playing)
	{
		img.draw();

		//draw mapthings
		for each (Sprite* thing in things)
		{
			mvwaddch(screen, thing->pos.y, thing->pos.x, thing->thing->symbol);
		}

		mvwaddch(screen, control.pos.y, control.pos.x, control.thing->symbol);

		wnoutrefresh(screen);


		doupdate();
		int c = getch();

		switch (c)
		{
		case KEY_DOWN: control.pos.y++; break;
		case KEY_UP: control.pos.y--; break;
		case KEY_LEFT: control.pos.x--; break;
		case KEY_RIGHT: control.pos.x++; break;
		case KEY_ESC: playing = false; continue;
		}

		//assuming directional button was pressed, collision detection routine runs here
		for each (Sprite* thing in things)
		{
			if (control.pos.y == thing->pos.y && control.pos.x == thing->pos.x)//if item is stepped over
			{
				//a transference of data from the thing to inventory or similar should occur first
				//remove thing from screen
				things.remove(thing);
				break; //break after first collision
			}

		}

	}


}


void loadMapRoomTest()
{
	MapRoom room;
	room.name = "Labyrinth4";
	FilePath path("data", &room);
	
	path.load(room.name + ".map");
	room.setWindow(dupwin(stdscr));

	room.draw();

	doupdate();
	getch();
}

void spriteOnMapTest()
{
	//int screenHeight = 23;
	//int screenWidth = 51;
	//resize_term(screenHeight, screenWidth);

	//WINDOW* screen = dupwin(stdscr);

	//MapRoom room;
	//room.setWindow(screen);
	//room.setDimensions(screenHeight, screenWidth);

	//Image& img = room.getDisplay();
	//
	//ITwoDStorage<chtype>& tileMap = img.getTileMap();
	//tileMap.fill('.');

	//
	//Actor actor;
	//initDefaultActor(actor);
	//actor.stats.hp.setCurr(5);


	////actor.symbol = 

	//Pos cPos(screenHeight / 2, screenWidth / 2);
	//Sprite control;
	//control.pos = cPos;
	//control.thing = &actor;
	//control.symbol = '@' | COLOR_YELLOW_BOLD << TEXTCOLOR_OFFSET;

	//GameItem item;
	//item.name = "Potion";
	//item.cost = 3;
	//item.type = GameItemType::CONSUMABLE;
	//
	//GameItem money;
	//money.name = "Gold$";
	//money.cost = 50;
	//money.type = GameItemType::MONEY;
	//
	//Actor npc;
	//npc.name = "Alex";
	//
	//
	//Sprite thing1;
	//thing1.thing = &item;
	//thing1.pos.y = 3;
	//thing1.pos.x = 3;
	//thing1.symbol = defaultItemSymbol;

	//Sprite thing2;
	//thing2.thing = &money;
	//thing2.pos.y = 20;
	//thing2.pos.x = 20;
	//thing2.symbol = '$' | COLOR_YELLOW_BOLD << TEXTCOLOR_OFFSET;

	//Sprite thing3;
	//thing3.thing = &npc;
	//thing3.pos.y = 10;
	//thing3.pos.x = 20;
	//thing3.symbol = '@' | COLOR_CYAN << TEXTCOLOR_OFFSET;

	//room.sprites.push_back(&thing1);
	//room.sprites.push_back(&thing2);
	//room.sprites.push_back(&thing3);

	//int totalGold = 0;
	//
	//bool playing = true;
	//while (playing)
	//{
	//	room.draw();

	//	

	//	mvwaddch(screen, control.pos.y, control.pos.x, control.symbol);
	//	mvwprintw(screen, screenHeight - 2, screenWidth - 10, "$:%-10u", totalGold);
	//	mvwprintw(screen, screenHeight - 3, screenWidth - 10, "HP:%+4u", actor.stats.hp.getCurr());

	//	wnoutrefresh(screen);


	//	doupdate();
	//	int c = getch();

	//	switch (c)
	//	{
	//	case KEY_DOWN: control.pos.y++; break;
	//	case KEY_UP: control.pos.y--; break;
	//	case KEY_LEFT: control.pos.x--; break;
	//	case KEY_RIGHT: control.pos.x++; break;
	//	case KEY_ESC: playing = false; continue;
	//	}

	//	//assuming directional button was pressed, collision detection routine runs here
	//	
	//	Sprite* s = room.checkCollisionDetection(control.pos);
	//	if (s != nullptr)
	//	{
	//		if (GameItem* item = dynamic_cast<GameItem*>(s->thing))
	//		{

	//			
	//			switch (item->type)
	//			{
	//			case GameItemType::MONEY: 
	//				totalGold += item->cost;
	//				room.sprites.remove(s);
	//				break;
	//			case GameItemType::CONSUMABLE:
	//				//
	//				((Actor*)control.thing)->alterStat(StatType::HP, 50);
	//				break;
	//			}
	//			room.sprites.remove(s);
	//		}
	//		else if (Actor* actor = dynamic_cast<Actor*>(s->thing))
	//		{
	//			control.pos.x++; //do not allow movement on top of npc

	//		}
	//	}

	//}


}