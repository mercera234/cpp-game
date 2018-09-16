#pragma once

#include "curses.h"
#include "panel.h"
#include <iostream>

//ctrl codes(not all of these have been tested!)
#define CTRL_AT 0x0 //couldn't use the symbol @
#define CTRL_A 0x1
#define CTRL_B 0x2
#define CTRL_C 0x3
#define CTRL_D 0x4
#define CTRL_E 0x5
#define CTRL_F 0x6
#define CTRL_G 0x7

//CTRL h, i, and j print nothing
#define CTRL_H KEY_BACKSPACE //ncurses test program showed these 2 equivalent?
#define CTRL_I 0x9 //8 space tab
#define CTRL_J 0xA //I suspect this is delete because this won't print out

#define CTRL_K 0xB
#define CTRL_L 0xC
#define CTRL_M 0xD //new line
#define CTRL_N 0xE
#define CTRL_O 0xF
#define CTRL_P 0x10
#define CTRL_Q 0x11
#define CTRL_R 0x12
#define CTRL_S 0x13
#define CTRL_T 0x14
#define CTRL_U 0x15
#define CTRL_V 0x16
#define CTRL_W 0x17
#define CTRL_X 0x18
#define CTRL_Y 0x19
#define CTRL_Z 0x1A
#define KEY_ESC 0x1B
#define CTRL_LEFT_BRACKET 0x1B
#define CTRL_BACKSLASH 0x1C
#define CTRL_RIGHT_BRACKET 0x1D
#define CTRL_CARET 0x1E
#define CTRL_UNDERSCORE 0x1F
//0X20 - 0X7E are printable characters (punc, alpha, numbers)
#define CTRL_QUESTION 0x7F
#define KEY_DEL 0x7F
#define KEY_PGUP KEY_PPAGE
#define KEY_PGDN KEY_NPAGE

#define MAX_ROWS 16
#define MAX_COLS 64

#define BORDER_OFFSET 0 //extra space needed when performing window operations with a border

#define COLOR_BOLD 0x8
#define COLOR_GRAY 8
#define COLOR_BLACK_BOLD COLOR_GRAY
#define COLOR_BLUE_BOLD 9
#define COLOR_GREEN_BOLD 10
#define COLOR_CYAN_BOLD 11
#define COLOR_RED_BOLD 12
#define COLOR_MAGENTA_BOLD 13
#define COLOR_YELLOW_BOLD 14
#define COLOR_WHITE_BOLD 15
#define COLOR_GRAY_BOLD 16 //doesn't work
#define TOTAL_COLORS 16

//color offsets
#define BKGDCOLOR_OFFSET 28
#define TEXTCOLOR_OFFSET 24

//bit masks
#define BKGDCOLOR_MASK 0xf0000000
#define TEXTCOLOR_MASK 0x0f000000
#define COLOR_MASK 0xff000000
#define ATTR_ONLY_MASK 0x00ff0000

/* Return a chtype tile with the background set to 'color' */
chtype setBkgdColor(int color);

/* Return the background color value from the tile*/
int getBkgdColor(chtype tile);

/* Return a chtype tile with the text set to 'color' */
chtype setTextColor(int color);

/* Return the text color value from the tile*/
int getTextColor(chtype tile);

//cursor states
enum class CursorType
{
	INVISIBLE = 0,
	NORMAL = 1,
	BOLD = 2
};

const unsigned short screenHeight = 23;
const unsigned short screenWidth = 51;
const unsigned short totalScreenTiles = screenHeight * screenWidth;
const std::string colorNames[16] = {
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

void setCursorType(CursorType typeIn);

/*Get the mouse event following a mouse click.
Will read as all 0s if no mouse click was made.*/
MEVENT* getMouse();

/*True allows user to simulate mouse input manually. 
Mouse event properties are reset on each use.*/
void simulateMouseOn(bool on);

/*manually set the mouse event. 
Useless without calling simulateMouseOn first.*/
void setMouseEvent(int y, int x);

class TUI
{
private:
	void initColors();
	
public:
	static void printOnBkgd(chtype c, WINDOW* win, int y, int x); //TUI must be initialized first to use this of course
	static void printStrOnBkgd(std::string s, WINDOW* win, int y, int x);

	/*Resizes the terminal to height and width and centers on desktop*/
	static void centerConsoleWindow(int height, int width);
	
	static bool simulateMouse; //true if mouse events are being set manually for testing
	static MEVENT mouseEvent; //the one mouse event in the program
	static CursorType cursorType;// = CursorType::INVISIBLE;
	TUI();

	WINDOW* win;
	void init();
	void shutdown(); //shutdown curses and other functions


	
};