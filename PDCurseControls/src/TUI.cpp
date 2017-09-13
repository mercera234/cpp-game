#include "TUI.h"
#include <iostream>
using namespace std;



TUI::TUI()
{
	
}

void TUI::init()
{
	initscr();                    /* Start curses mode */
	nonl();         /* tell curses not to do NL->CR/NL on output */
	
					//by enabling raw, Ctrl-C doesn't kill the program anymore!
	raw(); //enables the use of all ctrl codes
	noecho();         //turn echo off

	keypad(stdscr, true);//set stdscr as default for keypad (also allows use of arrow keys)
	initColors();

	mousemask(ALL_MOUSE_EVENTS, NULL); //allow mouse usage
	refresh();//do this once so that first call to getch doesn't do this
}

void TUI::initColors()
{
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

