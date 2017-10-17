#include "menu_tests.h"
#include "TUI.h"
#include "GridMenu.h"
#include "LineItem.h"
#include "ScrollBar.h"
#include "Palette.h"

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
			MenuItem* item = AbstractMenu::basicMenuDriver(input, &menu);

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
				mvaddch(7, 30, (chtype)item->crossRef);
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

			switch (item->index)
			{
			default: //set up swap ability here
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

		MenuItem* item = AbstractMenu::basicMenuDriver(input, &menu);

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

			mvaddch(7, 30, (chtype)item->crossRef);
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