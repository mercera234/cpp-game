#include "menu_tests.h"
#include "TUI.h"
#include "GridMenu.h"
#include "LineItem.h"
#include "ScrollBar.h"
#include "Palette.h"
#include "RGBControl.h"
#include "CursesAttributeController.h"

void gridMenuTest()
{
	int rows = 12;
	int cols = 1;
	WINDOW* win = newwin(3, 1 * 19, 1, 1);

	GridMenu menu(win, rows, cols);

	menu.setItem(new LineItem("0123456789012345", 0, 'N'));
	menu.setItem(new LineItem("TOGGLE WRAP", 1, 'L'));
	menu.setItem(new LineItem("QUIT", 2, 'Q'));
	menu.setItem(new LineItem("", 3, -1));
	menu.setItem(new LineItem("4123456789012345", 4, '?'));
	menu.setItem(new LineItem("Clear", 5, 'N'));
	menu.setItem(new LineItem("5123456789012345", 6, 'L'));
	menu.setItem(new LineItem("7123456789012345", 7, 'Q'));
	menu.setItem(new LineItem("Reset Items", 8, 'T'));
	menu.setItem(new LineItem("0 capacity", 9, '?'));
	menu.setItem(new LineItem("a123456789012345", 10, 'N'));
	menu.setItem(new LineItem("b123456789012345", 11, 'L'));

	menu.setCurrentItem(0);
	bool wrap = true;
	//menu.setRowSepLength(1);
	menu.setColorPair(COLOR_PAIR(COLOR_GREEN_BOLD) | COLOR_GREEN << 28);
	menu.post(true);
	bool usingMenu = true;
	while (usingMenu)
	{
		menu.draw();
		doupdate();
		int input = getch();

		switch (input) //process global input
		{
		case KEY_ESC: usingMenu = false; break;
		default:
		{
			LineItem* item = (LineItem*)AbstractMenu::basicMenuDriver(input, &menu);

			if (item == NULL)
			{
				continue;
			}

			switch (item->index)
			{

			case 1:
				menu.setWrapAround(wrap = !wrap); break;
			case 2:
				usingMenu = false;
				break;

			case 5:
				menu.clearItems();
				break;
			case 8:
				menu.resetItems(2, 2);
				menu.setItem(new LineItem("A", 0, -1));
				menu.setItem(new LineItem("B", 1, -1));
				menu.setItem(new LineItem("C", 2, -1));
				menu.setItem(new LineItem("D", 3, -1));
				menu.setCurrentItem(0);
				menu.post(true);
				break;
			case 9:
				//menu.resetItems(0, 0); //both of these statements work the same
				menu.resetItems(0, 1);

				break;
			default:
				mvaddch(7, 30, (chtype)item->getCrossRef());
				break;
			}
		}
		break;
		}
	}
}

void titleMenuTest()
{
	GridMenu titleMenu(stdscr, 4, 1);

	titleMenu.setItem(new LineItem("New Game", 0, 'N'));
	titleMenu.setItem(new LineItem("Load Game", 1, 'L'));
	titleMenu.setItem(new LineItem("Quit Game", 2, 'Q'));
	titleMenu.setItem(new LineItem("Edit Game", 3, 'E'));

	titleMenu.setWrapAround(false);
	titleMenu.setFocus(true);
	titleMenu.post(true);

	bool usingMenu = true;
	while (usingMenu)
	{
		titleMenu.draw();
		doupdate();
		int input = getch();

		switch (input) //process global input
		{
		case KEY_ESC: usingMenu = false; break;
		default:
		{
			MenuItem* item = AbstractMenu::basicMenuDriver(input, &titleMenu);

			if (item == nullptr)
			{
				continue;
			}

			switch (item->index)
			{
			case 2:
				usingMenu = false;
				break;
			default:
				break;
			}
		}
		break;
		}
	}
}

void mainMenuTest()
{
	GridMenu mainMenu(stdscr, 8, 1);

	mainMenu.setItem(new LineItem("Item", 0, 'I'));
	mainMenu.setItem(new LineItem("Equip", 1, 'E'));
	mainMenu.setItem(new LineItem("Status", 2, 'S'));
	mainMenu.setItem(new LineItem("Skill", 3, 'K'));
	mainMenu.setItem(new LineItem("Config", 4, 'C'));
	mainMenu.setItem(new LineItem("Map", 5, 'M'));
	mainMenu.setItem(new LineItem("Save", 6, 'V'));
	mainMenu.setItem(new LineItem("Quit", 7, 'Q'));

	mainMenu.setWrapAround(false);
	mainMenu.setFocus(true);
	mainMenu.post(true);

	bool usingMenu = true;
	while (usingMenu)
	{
		mainMenu.draw();
		doupdate();
		int input = getch();

		switch (input) 
		{
		case KEY_ESC: usingMenu = false; break;
		default:
		{
			MenuItem* item = AbstractMenu::basicMenuDriver(input, &mainMenu);

			if (item == nullptr)
			{
				continue;
			}

			switch (item->index)
			{
			case 7:
				usingMenu = false;
				break;
			default:
				break;
			}
		}
		break;
		}
	}
}


void itemMenuTest()
{
	int inventorySpace = 20;
	GridMenu itemMenu(stdscr, inventorySpace, 1);

	for (int i = 0; i < 20; i++)
	{
		itemMenu.setItem(new LineItem("               ", i, 0)); //set remaining inventory spaces to blank
	}

	itemMenu.setItem(new LineItem("Cell Water", 0, 'C'));
	itemMenu.setItem(new LineItem("Tazer", 10, 'T'));
	itemMenu.setItem(new LineItem("Laser Sword", 19, 'L'));
	
	itemMenu.setWrapAround(false);
	itemMenu.setFocus(true);
	itemMenu.post(true);

	bool usingMenu = true;
	bool swapState = false;
	int swapIndex;
	while (usingMenu)
	{
		itemMenu.draw();
		doupdate();
		int input = getch();

		switch (input)
		{
		case KEY_ESC: usingMenu = false; break;
		case '\r':
		case '\n':
		case KEY_ENTER:
		{
			MenuItem* item = AbstractMenu::basicMenuDriver(input, &itemMenu);

			if (item == nullptr)
			{
				continue;
			}

			if (item->index)
			{
			//set up swap ability here
				if (swapState == false)
				{
					item->selected = true;
					swapIndex = item->index;
					swapState = true;
				}
				else
				{
					unsigned short origIndex = item->index;
					itemMenu.swapItems(item->index, swapIndex);
					itemMenu.setCurrentItem(origIndex);
					swapState = false;
					
					MenuItem* item2 = itemMenu.AbstractMenu::getItem(origIndex);
					item2->selected = false;
				}

				break;
			}
		}
			break;
		default: 
			AbstractMenu::basicMenuDriver(input, &itemMenu);
			break;
		}
	}
}

void paletteTest()
{
	int rows = 4;
	int cols = 4;

	Palette palette(1, 1, "Test", rows, cols);

	for (int i = 0; i < TOTAL_COLORS; i++)
	{
		chtype c = ' ' | (i << 28) & A_COLOR;		
		palette.setItem(colorNames[i], c, i);
	}

	palette.setFocus(true);
	palette.post(true);

	bool usingMenu = true;
	while (usingMenu)
	{
		palette.draw();
		doupdate();
		int input = getch();

		switch (input)
		{
		case KEY_ESC: usingMenu = false; break;
		case '\r':
		case '\n':
		case KEY_ENTER:
		{
			palette.driver(input);
			
			LineItem* item = palette.getCurrentItem();
			
			if (item == nullptr)
			{
				continue;
			}

			
		}
		break;
		case KEY_MOUSE:
		{
			palette.driver(input);
		}
		break;
		default:
			palette.driver(input);
			break;
		}
	}
}

void palettePrototypeTest()
{
	int rows = 4;
	int cols = 4;

	//might make sense to use a factory method
	WINDOW* win = newwin(rows, cols * 3, 0, 0);
	WINDOW* label = newwin(1, 16, rows, 0);
	GridMenu palette(win, rows, cols);
	palette.setCursor(shortCursor);
	palette.setItemWidth(1);
	palette.setAcceptsMouseInput(true);
	palette.setWrapAround(false);
	palette.setFocus(true);

	for (int i = 0; i < TOTAL_COLORS; i++)
	{
		LineItem* item = new LineItem(colorNames[i], i, -1);
		item->setField(Fielddraw::DRAW_ICON);

		chtype c = ' ' | (i << 28) & A_COLOR;
		item->setIcon(c);

		palette.setItem(item);
	}


	palette.post(true);

	bool usingMenu = true;
	while (usingMenu)
	{
		palette.draw();
		doupdate();
		int input = getch();

		switch (input)
		{
		case KEY_ESC: usingMenu = false; break;
		case '\r':
		case '\n':
		case KEY_ENTER:
		{
			MenuItem* item = AbstractMenu::basicMenuDriver(input, &palette);

			if (item == nullptr)
			{
				continue;
			}


		}
		break;
		case KEY_MOUSE:
		{
			LineItem* item = (LineItem*)AbstractMenu::basicMenuDriver(input, &palette);
			wclear(label);
			int ret = mvwaddstr(label, 0, 0, item->name.c_str());
			int wret = wnoutrefresh(label);
			int x = 3;
		}
		break;
		default:
			AbstractMenu::basicMenuDriver(input, &palette);
			break;
		}
	}

	delwin(label);

}


void scrollMenuTest()
{
	int rows = 12;
	int cols = 1;
	WINDOW* win = newwin(4, (cols) * 19, 1, 1);

	GridMenu menu(win, rows, cols);
	menu.setItem(new LineItem("0123456789012345", 0, 'N'));
	menu.setItem(new LineItem("TOGGLE WRAP", 1, 'L'));
	menu.setItem(new LineItem("QUIT", 2, 'Q'));
	menu.setItem(new LineItem("3123456789012345", 3, 'T'));
	menu.setItem(new LineItem("4123456789012345", 4, '?'));
	menu.setItem(new LineItem("Clear", 5, 'N'));
	menu.setItem(new LineItem("Flip Mark", 6, 'L'));
	menu.setItem(new LineItem("7123456789012345", 7, 'Q'));
	menu.setItem(new LineItem("8123456789012345", 8, 'T'));
	menu.setItem(new LineItem("9123456789012345", 9, '?'));
	menu.setItem(new LineItem("a123456789012345", 10, 'N'));
	menu.setItem(new LineItem("b123456789012345", 11, 'L'));

	menu.setItemWidth(16);
	menu.post(true);
	//menu.disableItem(8, 0);
	bool wrap = true;

	bool usingMenu = true;

	ScrollBar* sb = new ScrollBar(&menu);

	while (usingMenu)
	{
		//menu.draw();
		sb->draw();
		doupdate();
		int input = getch();

		LineItem* item = (LineItem*)AbstractMenu::basicMenuDriver(input, &menu);

		if (item == NULL)
		{
			continue;
		}

		switch (item->index)
		{

		case 2:
			usingMenu = false;
			break;
		case 1:
			menu.setWrapAround(wrap = !wrap); break;
		case 5:
			menu.clearItems();
			break;


		default:
			mvaddch(7, 30, (chtype)item->getCrossRef());
			break;
		}
	}
}

GridMenu* m1driver(AbstractMenu* m, int input) //simulated callback for menu
{
	WINDOW* win2 = newwin(1, 40, 1, 1);
	GridMenu* m2 = new GridMenu(win2, 1, 2);

	/*m2->setItem("Yes2", "", 0, 0);
	m2->setItem("No2", "", 1, 1);
	*/return m2;
}


void menuTest2()
{
	WINDOW* win1 = newwin(1, 40, 1, 1);
	GridMenu* m1 = new GridMenu(win1, 1, 2);

	/*m1->setItem("Yes", "", 0, 0);
	m1->setItem("No", "", 1, 1);*/

	GridMenu* menus[2];
	menus[0] = m1;
	int totalMenus = 1;
	GridMenu* activeMenu = m1;

	bool playing = true;

	while (playing)
	{
		for (int i = 0; i < totalMenus; i++)
		{
			menus[i]->draw(); //draw all potential objects
		}
		doupdate();
		int c = getch();

		if (activeMenu == m1) //this section should be the callback! We call a specific routine based on what the Controllable object is
		{
			switch (c)
			{
			case KEY_LEFT: activeMenu->driver(REQ_LEFT_ITEM); break;
			case KEY_RIGHT: activeMenu->driver(REQ_RIGHT_ITEM); break;
			case '\r': activeMenu->driver(REQ_TOGGLE_ITEM); break;
			}

			MenuItem* mi = activeMenu->getCurrentItem();
			if (mi != NULL)
			{
				switch (mi->index)
				{
				case 0: //create pop-up window
					activeMenu = m1driver(activeMenu, c);
					menus[1] = activeMenu;
					totalMenus++;
					break;
				case 1: playing = false; //quit on no
					break;
				}
			}

		}
		else //callback for second method
		{
			switch (c)
			{
			case KEY_LEFT: activeMenu->driver(REQ_LEFT_ITEM); break;
			case KEY_RIGHT: activeMenu->driver(REQ_RIGHT_ITEM); break;
			case '\r': activeMenu->driver(REQ_TOGGLE_ITEM); break;
			}

			MenuItem* mi = activeMenu->getCurrentItem();
			if (mi != NULL)
			{
				switch (mi->index)
				{
				case 0: //quit on yes
					playing = false;
					break;
				case 1:   //backout on no (the callback that handles this portion can deallocate the memory)
					delete activeMenu;
					activeMenu = menus[0];
					totalMenus--;
					break;
				}
			}
		}
	}
}


void RGBMenuDriver(AbstractMenu* menu, int input)
{
	switch (input)
	{
	case KEY_LEFT: break;
	case KEY_RIGHT:  break;
	case KEY_UP: menu->driver(REQ_UP_ITEM); break;
	case KEY_DOWN: menu->driver(REQ_DOWN_ITEM); break;
	default: break;
	}


	return;
}

void RGBTest()
{
	WINDOW* win = newwin(3, 3, 1, 3);
	GridMenu* rgbMenu = new GridMenu(win, 3, 1);

	rgbMenu->setItem(new LineItem("R", 0, 0));
	rgbMenu->setItem(new LineItem("G", 1, 1));
	rgbMenu->setItem(new LineItem("B", 2, 2));

	char* mark = new char[3];
	mark[0] = '*'; mark[1] = '*'; mark[2] = 0;

	bool redOn = false;
	bool greenOn = false;
	bool blueOn = false;

	bool inMenu = true;
	int offColumn = 7;
	int onColumn = 12;

	int currRow = 1;

	int color = 0;
	chtype c_red = COLOR_PAIR(COLOR_RED);
	chtype c_green = COLOR_PAIR(COLOR_GREEN);
	chtype c_blue = COLOR_PAIR(COLOR_BLUE);
	chtype attrs = 0x0; //combined attributes

	while (inMenu)
	{
		//draw stdscr
		clear();


		chtype x = getattrs(stdscr);

		mvaddstr(0, 7, "OFF  ON");

		int redColumn, greenColumn, blueColumn;
		redColumn = redOn ? onColumn : offColumn;
		greenColumn = greenOn ? onColumn : offColumn;
		blueColumn = blueOn ? onColumn : offColumn;

		mvaddstr(1, redColumn, mark);
		mvaddstr(2, greenColumn, mark);
		mvaddstr(3, blueColumn, mark);

		wnoutrefresh(stdscr);

		//draw menu
		rgbMenu->draw();
		doupdate();
		//input
		int c = getch();

		if (c == 'q')
			break;



		//process
		RGBMenuDriver(rgbMenu, c);

		//!!!!Broke code here!!!!
		//currRow = rgbMenu->getCurrentIndex() + 1;

		chtype oneAttr = 0x0; //single attribute
		if (c == KEY_RIGHT)
		{
			switch (currRow)
			{
			case 1:
				if (redOn == false)
				{
					redOn = true;
					oneAttr = c_red;
				}
				break;
			case 2:
				if (greenOn == false)
				{
					greenOn = true;
					oneAttr = c_green;
				}
				break;
			case 3:
				if (blueOn == false)
				{
					blueOn = true;
					oneAttr = c_blue;
				}
				break;
			}
			attrs |= oneAttr;
			attron(attrs); //attron doesn't work right! I have to pass in all desired attributes and can't set one at a time
		}
		else if (c == KEY_LEFT)
		{
			switch (currRow)
			{
			case 1:
				if (redOn)
				{
					redOn = false;
					oneAttr = c_red;
				}
				break;
			case 2:
				if (greenOn)
				{
					greenOn = false;
					oneAttr = c_green;
				}
				break;
			case 3:
				if (blueOn)
				{
					blueOn = false;
					oneAttr = c_blue;
				}
				break;
			}
			attrs &= (oneAttr ^ 0xffffffff);
			attroff(oneAttr);
		}

	}


}



void RGBTest2()
{
	WINDOW* win = newwin(4, 3, 1, 3);
	GridMenu* rgbMenu = new GridMenu(win, 4, 1);

	/*rgbMenu->setItem("R", "", 0, 0);
	rgbMenu->setItem("G", "", 1, 1);
	rgbMenu->setItem("B", "", 2, 2);
	rgbMenu->setItem("!", "", 3, 3);*/

	char* mark = new char[3];
	mark[0] = '*'; mark[1] = '*'; mark[2] = 0;

	RGBControl control;

	bool inMenu = true;
	int offColumn = 7;
	int onColumn = 12;

	int currRow = 1;

	while (inMenu)
	{
		//do processing
		int redColumn = control.isColorOn(COLOR_RED) ? onColumn : offColumn;
		int greenColumn = control.isColorOn(COLOR_GREEN) ? onColumn : offColumn;
		int blueColumn = control.isColorOn(COLOR_BLUE) ? onColumn : offColumn;
		int boldColumn = control.isBoldOn() ? onColumn : offColumn;

		//draw stdscr
		clear();

		chtype colorAttrs = control.getColors();
		if (colorAttrs > A_BOLD) //at least 1 color is set
			attron(colorAttrs);
		else //no colors are set
			attroff(0xff00000);

		if (control.isBoldOn())
			attron(A_BOLD);
		else
			attroff(A_BOLD);

		chtype y = getattrs(stdscr);

		mvaddstr(0, 7, "OFF  ON");
		mvaddstr(1, redColumn, mark);
		mvaddstr(2, greenColumn, mark);
		mvaddstr(3, blueColumn, mark);
		mvaddstr(4, boldColumn, mark);

		wnoutrefresh(stdscr);

		//draw menu
		rgbMenu->draw();
		doupdate();
		//input
		int c = getch();

		if (c == 'q')
			break;

		//process
		RGBMenuDriver(rgbMenu, c);

		//!!!!Broke code here
		//currRow = rgbMenu->getCurrentIndex() + 1;

		bool position;
		int color;
		if (c == KEY_RIGHT)
		{
			position = ON_POSITION;
			switch (currRow)
			{
			case 1: color = COLOR_RED; break;
			case 2: color = COLOR_GREEN; break;
			case 3: color = COLOR_BLUE; break;
			case 4: color = A_BOLD; break;
			}
			control.pullSwitch(color, position);
		}
		else if (c == KEY_LEFT)
		{
			position = OFF_POSITION;
			switch (currRow)
			{
			case 1: color = COLOR_RED; break;
			case 2: color = COLOR_GREEN; break;
			case 3: color = COLOR_BLUE; break;
			case 4: color = A_BOLD; break;
			}
			control.pullSwitch(color, position);
		}

	}
}



void AttrControllerTest()
{
	WINDOW* win = newwin(4, 3, 1, 3);
	GridMenu* menu = new GridMenu(win, 4, 1);

	/*menu->setItem("R", "", 0, 0);
	menu->setItem("G", "", 1, 1);
	menu->setItem("B", "", 2, 2);
	menu->setItem("!", "", 3, 3);
	*/
	char* mark = new char[3];
	mark[0] = '*'; mark[1] = '*'; mark[2] = 0;

	CursesAttributeController control;

	bool inMenu = true;
	int offColumn = 7;
	int onColumn = 12;

	int currRow = 1;

	while (inMenu)
	{
		//do processing
		int redColumn = control.isColorStored(COLOR_RED) ? onColumn : offColumn;
		int greenColumn = control.isColorStored(COLOR_GREEN) ? onColumn : offColumn;
		int blueColumn = control.isColorStored(COLOR_BLUE) ? onColumn : offColumn;
		int boldColumn = control.isAttrStored(A_BOLD) ? onColumn : offColumn;

		//draw stdscr
		clear();

		//set colors
		chtype colorAttrs = control.getColors();
		if (control.getColors() > 0) //at least 1 color is set
			attron(colorAttrs);
		else //no colors are set
			attroff(A_COLOR);

		//set attributes
		chtype attrs = control.getAttrs();
		if (attrs > 0)
			attron(attrs);
		else
			attroff(A_ATTR_ONLY);

		chtype y = getattrs(stdscr);

		mvaddstr(0, 7, "OFF  ON");
		mvaddstr(1, redColumn, mark);
		mvaddstr(2, greenColumn, mark);
		mvaddstr(3, blueColumn, mark);
		mvaddstr(4, boldColumn, mark);

		wnoutrefresh(stdscr);

		//draw menu
		menu->draw();
		doupdate();
		//input
		int c = getch();

		if (c == 'q')
			break;

		//process
		RGBMenuDriver(menu, c);

		//!!!!!!Broke code here!!!
		//	currRow = menu->getCurrentIndex() + 1;

		int color;
		if (c == KEY_RIGHT)
		{
			switch (currRow)
			{
			case 1: color = COLOR_RED; control.storeColor(color); break;
			case 2: color = COLOR_GREEN; control.storeColor(color); break;
			case 3: color = COLOR_BLUE; control.storeColor(color); break;
			case 4: color = A_BOLD; control.storeAttribute(A_BOLD); break;
			}

		}
		else if (c == KEY_LEFT)
		{
			switch (currRow)
			{
			case 1: color = COLOR_RED; control.freeColor(color); break;
			case 2: color = COLOR_GREEN; control.freeColor(color); break;
			case 3: color = COLOR_BLUE; control.freeColor(color); break;
			case 4: color = A_BOLD; control.freeAttribute(A_BOLD); break;
			}
		}

	}
}


MenuItem* absMenuDriver(int input, AbstractMenu* m)
{
	MenuItem* item = NULL;
	int retval = -1;
	switch (input)
	{
	case KEY_DOWN: retval = m->driver(REQ_DOWN_ITEM); break;
	case KEY_UP: retval = m->driver(REQ_UP_ITEM); break;
	case KEY_LEFT: retval = m->driver(REQ_LEFT_ITEM); break;
	case KEY_RIGHT: retval = m->driver(REQ_RIGHT_ITEM); break;
	case KEY_PGDN: m->driver(REQ_SCR_DPAGE); break;
	case KEY_PGUP: m->driver(REQ_SCR_UPAGE); break;
	case '\n':
	case '\r':
	case KEY_ENTER:
		item = m->getCurrentItem();
		break;
	default: break;
	}

	return item;
}



//void graphMenuTest()
//{
//	GraphMenu* menu = new GraphMenu(newwin(10, 30, 1, 1), 4);
//	LineItem* li = new LineItem("item1", 0, -1);
//	li->setPosition(3, 3);
//	ActorCard* ac = new ActorCard(createActor("hero.actr", AT_HUMAN), 1, -1);
//	ac->setPosition(6, 10);
//	menu->setItem(li);
//	menu->setItem(ac);
//	
//	menu->setCurrentItem(1);
//	ac->link(Dir::UP, li);
//
//	bool playing = true;
//	while (playing)
//	{
//		menu->draw();
//		doupdate();
//		int input = getch();
//
//		switch (input)
//		{
//		case KEY_ESC: playing = false; break;
//		default:
//		{
//			MenuItem* item = absMenuDriver(input, menu);
//
//			if (item == NULL)
//			{
//				continue;
//			}
//
//		}
//		break;
//		}
//
//	}
//
//	
//}

//void graphMenuTest2()
//{
//	GraphMenu* targetMenu = new GraphMenu(newwin(16, 40, 1,1), 16);
//	
//	ActorCard* p1 = new ActorCard(createActor("hero.actr", AT_HUMAN), 0, -1);
//	ActorCard* p2 = new ActorCard(createActor("hero.actr", AT_HUMAN), 2, -1);
//	ActorCard* p3 = new ActorCard(createActor("hero.actr", AT_HUMAN), 4, -1);
//	ActorCard* p4 = new ActorCard(createActor("hero.actr", AT_HUMAN), 6, -1);
//
//	ActorCard* e1 = new ActorCard(createActor("toadie.actr", AT_CPU), 1, -1);
//	ActorCard* e2 = new ActorCard(createActor("toadie.actr", AT_CPU), 3, -1);
//	ActorCard* e3 = new ActorCard(createActor("toadie.actr", AT_CPU), 5, -1);
//	ActorCard* e4 = new ActorCard(createActor("toadie.actr", AT_CPU), 7, -1);
//
//	ActorCard* eh1 = new ActorCard(NULL, 8, -1);
//	ActorCard* eh2 = new ActorCard(NULL, 9, -1);
//	ActorCard* eh3 = new ActorCard(NULL, 10, -1);
//	ActorCard* eh4 = new ActorCard(NULL, 11, -1);
//	eh1->hidden = eh2->hidden = eh3->hidden = eh4->hidden = true;
//
//	ActorCard* ph1 = new ActorCard(NULL, 12, -1);
//	ActorCard* ph2 = new ActorCard(NULL, 13, -1);
//	ActorCard* ph3 = new ActorCard(NULL, 14, -1);
//	ActorCard* ph4 = new ActorCard(NULL, 15, -1);
//	ph1->hidden = ph2->hidden = ph3->hidden = ph4->hidden = true;
//
//	int pLeft = 2;
//	int eLeft = pLeft + 20;
//
//	//set positions
//	p1->setPosition(0, pLeft);
//	p2->setPosition(4, pLeft);
//	p3->setPosition(8, pLeft);
//	p4->setPosition(12, pLeft);
//
//	e1->setPosition(0, eLeft);
//	e2->setPosition(4, eLeft);
//	e3->setPosition(8, eLeft);
//	e4->setPosition(12, eLeft);
//
//
//	//link players together
//	p1->link(Dir::DOWN, p2);
//	p2->link(Dir::DOWN, p3);
//	p3->link(Dir::DOWN, p4);
//
//	//link enemies together
//	e1->link(Dir::DOWN, e2);
//	e2->link(Dir::DOWN, e3);
//	e3->link(Dir::DOWN, e4);
//
//	//link players to enemies
//	p1->link(Dir::RIGHT, e1);
//	p2->link(Dir::RIGHT, e2);
//	p3->link(Dir::RIGHT, e3);
//	p4->link(Dir::RIGHT, e4);
//
//	//link hidden items
//	e1->link(Dir::RIGHT, eh1);
//	e2->link(Dir::RIGHT, eh2);
//	e3->link(Dir::RIGHT, eh3);
//	e4->link(Dir::RIGHT, eh4);
//
//	p1->link(Dir::LEFT, ph1);
//	p2->link(Dir::LEFT, ph2);
//	p3->link(Dir::LEFT, ph3);
//	p4->link(Dir::LEFT, ph4);
//
//	//add all items to menu
//	targetMenu->setItem(p1);
//	targetMenu->setItem(p2);
//	targetMenu->setItem(p3);
//	targetMenu->setItem(p4);
//	targetMenu->setItem(e1);
//	targetMenu->setItem(e2);
//	targetMenu->setItem(e3);
//	targetMenu->setItem(e4);
//	targetMenu->setItem(eh1);
//	targetMenu->setItem(eh2);
//	targetMenu->setItem(eh3);
//	targetMenu->setItem(eh4);
//	targetMenu->setItem(ph1);
//	targetMenu->setItem(ph2);
//	targetMenu->setItem(ph3);
//	targetMenu->setItem(ph4);
//
//	targetMenu->setCurrentItem(0);
//
//	bool playing = true;
//	while (playing)
//	{
//		targetMenu->draw();
//		doupdate();
//
//		int input = getch();
//
//		switch (input)
//		{
//		case KEY_ESC: playing = false; break;
//		default:
//		{
//			absMenuDriver(input, targetMenu);
//
//			MenuItem* item = targetMenu->getCurrentItem();
//			if (item == nullptr)
//			{
//				continue;
//			}
//			else if (item == eh1 || item == eh2 || item == eh3 || item == eh4)
//			{
//				targetMenu->setSelected(1, true);
//				targetMenu->setSelected(3, true);
//				targetMenu->setSelected(5, true);
//				targetMenu->setSelected(7, true);
//			}
//			else if (item == ph1 || item == ph2 || item == ph3 || item == ph4)
//			{
//				targetMenu->setSelected(0, true);
//				targetMenu->setSelected(2, true);
//				targetMenu->setSelected(4, true);
//				targetMenu->setSelected(6, true);
//			}
//			else
//			{
//				for(int i = 0; i < 8; i++) //clear all non-hidden items
//					targetMenu->setSelected(i, false);
//			}
//
//
//		}
//		break;
//		}
//	}
//	
//}
