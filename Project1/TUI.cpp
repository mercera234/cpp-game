#include "TUI.h"
#include <iostream>
using namespace std;



TUI::TUI()
{
	
}

string TUI::colorNames[] = {
	"Black", //0
	"Blue", //1
	"Green", //2
	"Cyan",
	"Red", //4
	"Magenta",
	"Yellow",
	"White", //7
	"Grey", //8
	"Bold Blue",
	"Bold Green",
	"Bold Cyan",
	"Bold Red",
	"Bold Magenta",
	"Bold Yellow",
	"Bold White" };

void TUI::init()
{
	initscr();                    /* Start curses mode */
	nonl();         /* tell curses not to do NL->CR/NL on output */
	
					//by enabling raw, Ctrl-C doesn't kill the program anymore!
	raw(); //enables the use of all ctrl codes
	noecho();         //turn echo off

	keypad(stdscr, true);//set stdscr as default for keypad (also allows use of arrow keys)
	if (has_colors())
	{
		start_color();

		/*Pair 0 will not be redefined.
		  Using the loop below, normally Pair 0 would be black on black, but any pair where text and background match is not necessary
		*/
		int pair = 0;
		for (int bgkd = 0; bgkd < 16; bgkd++)
		{
			for (int text = 0; text < 16; text++)
			{
				init_pair(pair++, text, bgkd);
			}
		}
	}

	mousemask(ALL_MOUSE_EVENTS, NULL); //allow mouse usage
	refresh();//do this once so that first call to getch doesn't do this
}


void TUI::testPrint()
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

void TUI::testColors()
{
	string text = "Color Test";

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



void TUI::gameTestScreen()
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

/*
Print a single character in a window so that the background doesn't change
*/
void TUI::printOnBkgd(chtype c, WINDOW* win, int y, int x)
{
	//strip text and text color from c
	chtype text = c & A_CHARTEXT;
	chtype regularC = c & TEXTCOLOR_MASK;
	chtype standoutC = COLOR_PAIR(COLOR_BLACK); //prepare standoutcolor if needed
	
	chtype bkgdTile = mvwinch(win, y, x);
	bkgdTile &= BKGDCOLOR_MASK;//keep only the background

	chtype printTextColor = (getBkgdColor(bkgdTile) != getTextColor(regularC) ? regularC : standoutC);
	waddch(win, bkgdTile | printTextColor | text);
}


void TUI::shutdown()
{
	endwin();
	// finish(0);               /* we're done */
}

