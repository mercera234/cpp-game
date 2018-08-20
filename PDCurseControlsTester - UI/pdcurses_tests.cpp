#include "pdcurses_tests.h"
#include "curses.h"
#include "panel.h"
//No user made classes should be included here
#include <iostream>
#include <dirent.h>
#include <fstream>
#include <string>

void resizeTest()
{
	int x = getmaxx(stdscr);
	int y = getmaxy(stdscr);

	doupdate();
	getch();

	resize_term(y / 2, x / 2);

	doupdate();
	getch();

}

void testPanel()
{
	WINDOW* panelWin[3];
	PANEL* panel[3];
	init_pair(1, COLOR_WHITE | 0x8, COLOR_BLUE);

	panelWin[0] = newwin(5, 25, 0, 0);
	panelWin[1] = newwin(5, 25, 2, 16);
	panelWin[2] = newwin(5, 25, 3, 32);

	wattron(panelWin[0], COLOR_PAIR(1));
	wattron(panelWin[1], COLOR_PAIR(1));
	wattron(panelWin[2], COLOR_PAIR(1));

	wbkgd(panelWin[0], COLOR_PAIR(1));
	wbkgd(panelWin[1], COLOR_PAIR(1));
	wbkgd(panelWin[2], COLOR_PAIR(1));

	box(panelWin[0], 0, 0);
	box(panelWin[1], 0, 0);
	box(panelWin[2], 0, 0);

	mvwaddch(panelWin[2], 1, 1, 'C');
	mvwaddch(panelWin[1], 1, 1, 'B');
	mvwaddch(panelWin[0], 1, 1, 'A');


	mvwaddch(panelWin[2], 3, 23, 'C');
	mvwaddch(panelWin[1], 3, 23, 'B');
	mvwaddch(panelWin[0], 3, 23, 'A');


	panel[0] = new_panel(panelWin[0]);
	panel[1] = new_panel(panelWin[1]);
	panel[2] = new_panel(panelWin[2]);
	update_panels();

	doupdate();
	getch();

	top_panel(panel[1]);
	update_panels();
	doupdate();
	getch();

	top_panel(panel[0]);
	update_panels();
	doupdate();
	getch();


}

void overlapWindowTest()
{
	int totalRows = getmaxy(stdscr);
	int totalCols = getmaxx(stdscr);

	WINDOW* win = newwin(totalRows, totalCols, 0, 0);
	wbkgd(win, 'A');

	wnoutrefresh(win);
	doupdate(); //terminal is filled with A
	getch();

	WINDOW* win2 = newwin(totalRows, totalCols, 0, 0);
	wbkgd(win2, 'B');

	wnoutrefresh(win2);
	doupdate(); //terminal is filled with B
	getch();

	//touchwin must occur before refresh or else the first window won't show up again
	touchwin(win); 
	wnoutrefresh(win);
	doupdate();
	getch();
}

void overlapWindowTest2()
{
	int totalRows = getmaxy(stdscr);
	int totalCols = getmaxx(stdscr);

	WINDOW* win = newwin(totalRows, totalCols, 0, 0);

	int totalTiles = totalRows * totalCols;
	for (int i = 0; i < totalTiles; i++)
	{
		waddch(win, 'A');
	}
	
	wnoutrefresh(win);
	doupdate();
	getch();

	WINDOW* win2 = newwin(totalRows, totalCols, 0, 0);
	for (int i = 0; i < totalTiles; i++)
	{
		waddch(win2, 'B');
	}

	wnoutrefresh(win2);
	doupdate();
	getch();

	//touchwin must occur before refresh or else the first window won't show up again
	touchwin(win);
	wnoutrefresh(win);
	doupdate();
	getch();
}

void overlapWindowTest3()
{
	int totalRows = getmaxy(stdscr);
	int totalCols = getmaxx(stdscr);

	WINDOW* win = newwin(totalRows, 30, 0, 0);

	int totalTiles = totalRows * 30;
	for (int i = 0; i < totalTiles; i++)
	{
		waddch(win, 'A');
	}

	WINDOW* win2 = newwin(totalRows, 30, 0, 20);
	for (int i = 0; i < totalTiles; i++)
	{
		waddch(win2, 'B');
	}

	wnoutrefresh(win);
	wnoutrefresh(win2);
	
	doupdate();
	getch();

	werase(win);
	for (int i = 0; i < totalTiles; i++)
	{
		int x = waddch(win, 'A'); //returns an error if win is not erased!
		if(x == 0)
			int y = 3;
	}
	wnoutrefresh(win); 
	doupdate();
	getch();

	
	/*using only B is not recognized as a change to the window. 
	The next refresh statement will not show the B, unless the window is 'touched' first.
	But even then, everything in win2 is shown overlapping winA, and not just the one B we altered*/
	mvwaddch(win2, 0, 0, 'B');
	touchwin(win2); 
	wnoutrefresh(win2);
	doupdate();
	getch();
}


void newColorTest()
{
	char* text = "Here is text";

	int maxy = getmaxy(stdscr);



	for (int pair = 0; pair < 16; pair++)
	{
		chtype attr = 0x00010000;

		mvprintw(0, 0, "Color Pair: 0x%x", pair);
		for (int i = 0; i < 16; i++)
		{
			chtype color = COLOR_PAIR(pair);
			chtype c = attr | color;
			attron(c);
			mvprintw(i + 1, 0, "%d) %s: 0x%x", i, text, c);
			attroff(c);
			attr <<= 1;
			//attr &= 0xfffffffe;
		}
		refresh();
		doupdate();
		getch();
	}

	//reverse colors
	init_pair(1, COLOR_BLACK, COLOR_BLUE);
	init_pair(2, COLOR_BLACK, COLOR_GREEN);
	init_pair(3, COLOR_BLACK, COLOR_CYAN);
	init_pair(4, COLOR_BLACK, COLOR_RED);
	init_pair(5, COLOR_BLACK, COLOR_MAGENTA);
	init_pair(6, COLOR_BLACK, COLOR_YELLOW);
	init_pair(7, COLOR_BLACK, COLOR_WHITE); //identical to no color pair used

	for (int i = 8; i < COLORS; i++)
	{
		init_pair(i, COLOR_BLACK, i);
	}

	for (int pair = 0; pair < 16; pair++)
	{
		chtype attr = 0x00010000;

		mvprintw(0, 0, "Color Pair: 0x%x", pair);
		for (int i = 0; i < 16; i++)
		{
			chtype color = COLOR_PAIR(pair);
			chtype c = attr | color;
			attron(c);
			mvprintw(i + 1, 0, "%d) %s: 0x%x", i, text, c);
			attroff(c);
			attr <<= 1;
			//attr &= 0xfffffffe;
		}
		refresh();
		doupdate();
		getch();
	}

	//mix colors
	init_pair(1, COLOR_GREEN, COLOR_BLUE);
	init_pair(2, COLOR_RED, COLOR_GREEN);
	init_pair(3, COLOR_YELLOW, COLOR_CYAN);
	init_pair(4, COLOR_MAGENTA, COLOR_RED);
	init_pair(5, COLOR_BLUE, COLOR_MAGENTA);
	init_pair(6, COLOR_CYAN, COLOR_YELLOW);
	init_pair(7, COLOR_YELLOW, COLOR_WHITE); //identical to no color pair used

	for (int i = 8; i < COLORS; i++)
	{
		init_pair(i, COLOR_RED, i);
	}

	for (int pair = 0; pair < 16; pair++)
	{
		chtype attr = 0x00010000;

		mvprintw(0, 0, "Color Pair: 0x%x", pair);
		for (int i = 0; i < 16; i++)
		{
			chtype color = COLOR_PAIR(pair);
			chtype c = attr | color;
			attron(c);
			mvprintw(i + 1, 0, "%d) %s: 0x%x", i, text, c);
			attroff(c);
			attr <<= 1;
			//attr &= 0xfffffffe;
		}
		refresh();
		doupdate();
		getch();
	}
}

void attrTest()
{
	chtype red = 0x04000000;
	chtype green = 0x02000000;
	attron(red | A_BOLD); //COLOR_PAIR(COLOR_RED));
	mvaddstr(0, 0, "dsaljksdfl");
	doupdate();
	getch();

	attron(green); // COLOR_PAIR(COLOR_GREEN));
	mvaddstr(1, 0, "DSLDSL");

	doupdate();
	getch();

	attron(green | red); // COLOR_PAIR(COLOR_GREEN));
	mvaddstr(2, 0, "DSLDSL");

	doupdate();
	getch();
}

void colorPairTest()
{
	int pair = 0;
	for (int bgkd = 0; bgkd < 16; bgkd++)
	{
		for (int text = 0; text < 16; text++)
		{
			init_pair(pair++, text, bgkd);
		}
	}

	int row = 0;
	for (int i = 0; i < 256; i++)
	{
		attron(COLOR_PAIR(i));
		chtype attrs = getattrs(stdscr);
		mvprintw(row++, 0, "%d) Color test: 0x%x", i, attrs);

		if (row % 16 == 0)
		{
			row = 0;
			doupdate();
			getch();
		}

	}
}

void testPrint()
{
	int range = 256;

	resize_term(range, 100);
	//resize_window(stdscr, range, 100);

	int offset = 10;
	int col = 0;
	int row = 0;
	for (int i = 0; i < range; i++)
	{
		mvaddch(row, col, i);
		addch(' ');
		printw("0x%x", i);
		row++;
		if (row > 25)
		{
			row = 0;
			col += offset;
		}

	}

	refresh();                    /* Print it on to the real screen */
	getch();                      /* Wait for user input */
}

void testColors()
{
	std::string text = "Color Test";

	int row = 0;



	int color_pairs = COLORS;

	for (int row = 0; row < color_pairs; row++)
	{
		attron(COLOR_PAIR(row));
		mvprintw(row, 0, "%u %s", row, text.c_str());
		attroff(COLOR_PAIR(row));
	}

	attron(A_BOLD);
	for (int row = 0; row < color_pairs; row++)
	{
		attron(COLOR_PAIR(row));
		mvprintw(row, 16, "%u %s", row, text.c_str());
		attroff(COLOR_PAIR(row));
	}

	refresh();                    /* Print it on to the real screen */
	getch();

	init_pair(1, COLOR_RED, 14);
	for (int row = 0; row < color_pairs; row++)
	{
		attron(COLOR_PAIR(row));
		mvprintw(row, 32, "%u %s", row, text.c_str());
		attroff(COLOR_PAIR(row));
	}


	refresh();

	getch();                      /* Wait for user input */
}

void mouseTest()
{
	MEVENT event;
	mousemask(ALL_MOUSE_EVENTS, NULL);


	bool a = true;
	while (a)
	{
		int c = getch();
		switch (c)
		{
		case KEY_MOUSE:

			nc_getmouse(&event);
			if (event.bstate == BUTTON1_CLICKED)
			{
				mvaddstr(event.y, event.x, "BUTTON1_CLICKED"); //left click
			}
			else if (event.bstate == BUTTON2_CLICKED)
			{
				mvaddstr(event.y, event.x, "BUTTON2_CLICKED"); //wheel click
			}
			else if (event.bstate == BUTTON3_CLICKED)
			{
				mvaddstr(event.y, event.x, "BUTTON3_CLICKED"); //right click
			}

			//mvaddstr(0, 0, "Mouse");
			break;
		default:
			mvaddstr(0, 0, "Keyboard");
			break;
		}

		doupdate();
	}

}

void derWinTest()
{
	WINDOW* win = newwin(6, 24, 1, 1);

	int y = 2;
	int x = 8;
	WINDOW* dWin = derwin(win, 2, 8, y, x);


	bool playing = true;
	while (playing)
	{
		//clear();
		//wnoutrefresh(stdscr);

		mvwaddstr(win, 0, 0, "MAIN WINDOW");
		mvwaddstr(dWin, 0, 0, "SUBSUBSUB");
		//wbkgd(dWin, 'd' | 0x20000000); //why does this work when the derived window is set before the main window?!
		//wbkgd(win, 'w' | 0x40000000);

		wnoutrefresh(win);
		//wnoutrefresh(dWin); //draw and refresh subwin first?!

		doupdate();
		//keypad(win, true);
		//keypad(dWin, true);
		int c = getch();

		switch (c)
		{
		case KEY_UP: y--;  break;
		case KEY_DOWN: y++;  break;
		case KEY_LEFT: x--; break;
		case KEY_RIGHT: x++; break;
		case 'q': playing = false; break;
		}
		int d = mvderwin(dWin, 1, 1);
		int e = 5;
	}
}

void wideTest()
{
	int c = sizeof(char);
	int w = sizeof(wchar_t);


	wchar_t wc2 = 256;
	wchar_t wc3 = 0xffff;

	int rows = getmaxy(stdscr);
	int currRow = 0;
	for (int i = 0; i <= 65535; i++)
	{
		wchar_t wc = i;

		mvwprintw(stdscr, currRow, 0, "%d) %d c:%c s:%hc w:%lc", i, wc, wc, wc, wc);

		currRow++;
		if (currRow > rows)
		{
			wnoutrefresh(stdscr);
			doupdate();
			getch();
			clear();
			currRow = 0;
		}
	}

}

void objectFormTest()
{
	char buf[256];
	GetFullPathName(".", 256, buf, NULL);
	std::string fullPath(buf);

	std::ifstream is("data\\form_ActorDef.txt");
	//is.open();

	if (is.is_open() == false)
	{
		return;
	}

	std::string line;

	int i = 0;

	std::string type;
	std::getline(is, type); //get type

	std::string label;
	int length;
	std::string member;
	std::string validation;
	std::string range;

	while (is.eof() == false)
	{
		getline(is, line);
		is >> label >> length >> member >> validation >> range;

		if (member.compare("name") == 0)
		{
			//TODO
		}


		//mvaddstr(i++, 0, line.c_str());
		mvaddstr(i++, 0, validation.c_str());
	}

	wnoutrefresh(stdscr);
	doupdate();
	int input = getch();

	is.close();
}

void mockSimpleFight()
{
	clear();

	int nameRow = 5;
	int heroCol = 0;
	int enemyCol = 25;
	int hpRow = 6;


	mvaddstr(nameRow, heroCol, "Hero");
	mvaddstr(hpRow, heroCol, "HP 4/25");

	mvaddstr(nameRow, enemyCol, "Enemy");
	mvaddstr(hpRow, enemyCol, "HP 16/16");

	attron(COLOR_PAIR(12));
	mvaddstr(nameRow, 35, "-5");

	doupdate();
	getch();

	attroff(COLOR_PAIR(12));
	mvaddstr(nameRow, 35, "  ");
	mvaddstr(hpRow, enemyCol, "HP 11/16");
	attron(COLOR_PAIR(10));
	mvaddstr(nameRow, 10, "+20");

	doupdate();
	getch();


}

void mockBattleScreen()
{
	short totalRows = 23;
	short totalCols = 51;
	resize_term(totalRows, totalCols);
	WINDOW* frame = newwin(totalRows, totalCols, 0, 0);
	std::string ruler = "1234567890123456789012345678901234567890123456789012345678901234567890";
	std::string charName = "Stephanopoulise"; //15 char name max length
	std::string hpStats = "HP 1000/2000"; //max length of HP
	std::string mpStats = "MP 1000/2000"; //max length of MP

	int partyIndent = 1;
	int enemyIndent = partyIndent + 16 + 5 + 4;
	int startRow = 1;

	mvwaddnstr(frame, 0, 0, ruler.c_str(), totalCols);
	for (int i = 0; i < 4; i++)
	{
		mvwaddstr(frame, startRow, partyIndent, charName.c_str());
		mvwaddstr(frame, startRow + 1, partyIndent, hpStats.c_str());
		mvwaddstr(frame, startRow + 2, partyIndent, mpStats.c_str());

		startRow += 4;
	}

	startRow = 1;
	for (int i = 0; i < 4; i++)
	{
		mvwaddstr(frame, startRow, enemyIndent, charName.c_str());
		mvwaddstr(frame, startRow + 1, enemyIndent, hpStats.c_str());
		mvwaddstr(frame, startRow + 2, enemyIndent, mpStats.c_str());

		startRow += 4;
	}

	mvwaddstr(frame, 1, partyIndent + 16, "<-");
	mvwaddstr(frame, 5, partyIndent + 16, "-1234");

	mvwaddstr(frame, 1, enemyIndent + 16, "<-");
	mvwaddstr(frame, 5, enemyIndent + 16, "+2000");

	WINDOW* subPanel = newwin(6, totalCols, totalRows - 6, 0);
	box(subPanel, 0, 0);

	wnoutrefresh(frame);
	wnoutrefresh(subPanel);
	doupdate();
	getch();
}

void mockExploreTest()
{
	short totalRows = 23;
	short totalCols = 51;
	resize_term(totalRows, totalCols);
	WINDOW* frame = newwin(totalRows, totalCols, 0, 0);

	//box(frame, 0, 0);
	//                 012345678901234567890123456789012345678901234567890
	std::string rowType1 = "                       #...#                       ";
	std::string rowType2 = "########################...########################";
	std::string rowType3 = "...................................................";

	for (int row = 0; row < 9; row++)
	{
		mvwaddstr(frame, row, 0, rowType1.c_str());
	}

	mvwaddstr(frame, 9, 0, rowType2.c_str());
	mvwaddstr(frame, 10, 0, rowType3.c_str());
	mvwaddstr(frame, 11, 0, rowType3.c_str());
	mvwaddstr(frame, 12, 0, rowType2.c_str());

	for (int row = 13; row < totalRows; row++)
	{
		mvwaddstr(frame, row, 0, rowType1.c_str());
	}

	wnoutrefresh(frame);
	doupdate();
	getch();

	WINDOW* msgBox = newwin(6, totalCols, 0, 0);
	box(msgBox, 0, 0);

	int indent = 1;
	mvwaddstr(msgBox, 1, indent, "?????: This is an example of the type of place you may explore.");
	mvwaddstr(msgBox, 3, indent, "Alex: OK. Some more words here, blah blah lbah.");

	wnoutrefresh(msgBox);
	doupdate();
	getch();
}

void mockMainMenuTest()
{
	short totalRows = 23;
	short totalCols = 51;
	resize_term(totalRows, totalCols);

	int leftBoxWidth = 20;
	int topBoxHeight = 6;
	int startRow = 0;
	int startCol = 0;
	WINDOW* menuFrame = newwin(topBoxHeight, leftBoxWidth, startRow, startCol);

	chtype motif = ' ' | 0x17000000; //blue background, white text
	wbkgd(menuFrame, motif);
	box(menuFrame, 0, 0);

	mvwaddstr(menuFrame, 1, 3, "Item");
	mvwaddstr(menuFrame, 2, 3, "Equip");
	mvwaddstr(menuFrame, 3, 3, "Status");
	mvwaddstr(menuFrame, 4, 3, "Skill");
	mvwaddstr(menuFrame, 1, 11, "Config");
	mvwaddstr(menuFrame, 2, 11, "Map");
	mvwaddstr(menuFrame, 3, 11, "Save");
	mvwaddstr(menuFrame, 4, 11, "Quit");
	mvwaddstr(menuFrame, 1, 1, "->");
	
	int bottomBoxHeight = totalRows - topBoxHeight + 1;
	WINDOW* charFrame = newwin(bottomBoxHeight, leftBoxWidth, startRow + 5, startCol);
	wbkgd(charFrame, motif);
	box(charFrame, 0, 0);

	std::string charName = "Stephanopoulise"; //15 char name max length
	std::string hpStats = "HP 1000/2000"; //max length of HP
	std::string mpStats = "MP 1000/2000"; //max length of MP

	
	for (int i = 0, row = 1; i < 4; i++, row += 4)
	{
		mvwaddstr(charFrame, row, 3, charName.c_str());
		mvwaddstr(charFrame, row + 1, 3, hpStats.c_str());
		mvwaddstr(charFrame, row + 2, 3, mpStats.c_str());
	}

	int rightBoxWidth = totalCols - leftBoxWidth;
	WINDOW* descFrame = newwin(topBoxHeight, rightBoxWidth, startRow, startCol + leftBoxWidth);
	wbkgd(descFrame, motif);
	box(descFrame, 0, 0);
	
	mvwaddstr(descFrame, 1, 1, "Dungeon 1");
	mvwaddstr(descFrame, 2, 1, "Level B3");



	WINDOW* bodyFrame = newwin(bottomBoxHeight, rightBoxWidth, startRow + 5, startCol + leftBoxWidth);
	wbkgd(bodyFrame, motif);
	box(bodyFrame, 0, 0);

	mvwaddstr(bodyFrame, 1, 1, "Gold$ 3500");
	mvwaddstr(bodyFrame, 2, 1, "Steps 10023");
	mvwaddstr(bodyFrame, 3, 1, "Enemies Killed 380");
	mvwaddstr(bodyFrame, 4, 1, "Battles Won 250");



	wnoutrefresh(menuFrame);
	wnoutrefresh(charFrame);
	wnoutrefresh(descFrame);
	wnoutrefresh(bodyFrame);
	//wnoutrefresh(stdscr);
	doupdate();
	getch();
}
