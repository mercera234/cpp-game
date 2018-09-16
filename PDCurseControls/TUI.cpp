#include <iostream>
#include <Windows.h>
#include "TUI.h"

chtype setBkgdColor(int color)
{
	if (color < 0 || color >= TOTAL_COLORS)
		return (chtype)color; //return what was passed if an error

	return color << BKGDCOLOR_OFFSET;
}

int getBkgdColor(chtype tile)
{
	int color = tile >> BKGDCOLOR_OFFSET;

	if (color < 0 || color >= TOTAL_COLORS)
		return COLOR_BLACK; //return black if not a valid color

	return color;
}

chtype setTextColor(int color)
{
	if (color < 0 || color >= TOTAL_COLORS)
		return (chtype)color; //return what was passed if an error

	return color << TEXTCOLOR_OFFSET;
}

int getTextColor(chtype tile)
{
	int color = tile >> TEXTCOLOR_OFFSET;

	if (color < 0 || color >= TOTAL_COLORS)
		return COLOR_BLACK; //return black if not a valid color

	return color;
}


TUI::TUI()
{
	
}

CursorType TUI::cursorType = CursorType::INVISIBLE;
bool TUI::simulateMouse = false;
MEVENT TUI::mouseEvent;

void TUI::init()
{
	initscr();                    /* Start curses mode */
	nonl();         /* tell curses not to do NL->CR/NL on output */
	
					//by enabling raw, Ctrl-C doesn't kill the program anymore!
	raw(); //enables the use of all ctrl codes
	noecho();         //turn echo off

	keypad(stdscr, true);//set stdscr as default for keypad (also allows use of arrow keys)
	initColors();
	
	setCursorType(cursorType);

	mousemask(ALL_MOUSE_EVENTS, NULL); //allow mouse usage
	simulateMouseOn(false);//this should be false by default
	
	refresh();//do this once so that first call to getch doesn't do this
}


void setCursorType(CursorType typeIn)
{
	TUI::cursorType = typeIn;
	curs_set((int)TUI::cursorType); //this should not be called anywhere else
}

void simulateMouseOn(bool on)
{
	TUI::simulateMouse = on;
	TUI::mouseEvent.x = 0;
	TUI::mouseEvent.y = 0;
	TUI::mouseEvent.bstate = 0;
}

MEVENT* getMouse()
{
	if (TUI::simulateMouse == false)
		nc_getmouse(&TUI::mouseEvent);

	return &TUI::mouseEvent;
}

void setMouseEvent(int y, int x)
{
	TUI::mouseEvent.y = y;
	TUI::mouseEvent.x = x;
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

void TUI::printStrOnBkgd(std::string s, WINDOW* win, int y, int x)
{
	int col = x;
	for each (char c in s)
	{
		chtype ct = c | COLOR_WHITE << TEXTCOLOR_OFFSET;
		printOnBkgd(ct, win, y, col++);
	}
}

void TUI::centerConsoleWindow(int height, int width)
{
	resize_term(height, width); /*Always ensure that your terminal is sized to contain all windows that it renders or else window creation routines will return null*/

	HWND consoleWindow = GetConsoleWindow();
	RECT consoleRect;
	GetWindowRect(consoleWindow, &consoleRect);

	HWND desktopWindow = GetDesktopWindow();
	RECT desktopRect;
	GetWindowRect(desktopWindow, &desktopRect);

	int y = (desktopRect.bottom - (consoleRect.bottom - consoleRect.top)) / 2;
	int x = (desktopRect.right - (consoleRect.right - consoleRect.left)) / 2;

	SetWindowPos(consoleWindow, 0,
		x, y, //x, y
		0, 0, SWP_NOSIZE | SWP_NOZORDER);
}



void TUI::shutdown()
{
	endwin();
	// finish(0);               /* we're done */
}

