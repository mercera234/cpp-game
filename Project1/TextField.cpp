#include "TextField.h"
#include <sstream>
#include "TUI.h"

//I can't get rid of this method yet because it is needed if TextField is derived from another window
TextField::TextField(WINDOW* win)
{
	totalRows = 1;
	totalCols = maxLen = getmaxx(win);

	init(win);
}

/*
Right now, this assumes the window length and text max length are equal
*/
TextField::TextField(int length, int y, int x)
{
	totalRows = 1;
	totalCols = maxLen = length;

	WINDOW* w = newwin(totalRows, length, y, x);
	init(w);
}

void TextField::init(WINDOW* win)
{
	focusable = true;

	setWindow(win);
	getbegyx(win, scrY, scrX);

	//length = getmaxx(win);
	text = new char[maxLen]; //set one more so there is a null char at the end

	setEditable(true);

	memset(text, BLANK_SPACE, maxLen);
	curLen = 0; //length of text stored
	textPtr = text;
	cursorPos = 0; //starts at beginning

	color = 0x00000000; //set to black and white by default
}

void TextField::setFocus()
{
	curs_set(CURSOR_NORMAL);

	//apply move globally to screen
	//need to use :: to differentiate this from the Controllable move method
	::move(scrY, scrX + cursorPos); //a known issue with using wmove combined with getch is why we're using move
}

bool TextField::deleteChar()
{
	if (cursorPos >= curLen)
		return false;
	//shift string beyond position up a space
	short moveSize = curLen - (cursorPos + 1); //length of printable characters - one beyond current position
	memcpy_s(textPtr, moveSize, textPtr + sizeof(char), moveSize);
	//clear last position
	text[curLen - 1] = BLANK_SPACE;
	curLen--; //decrement current length
	return true;
}


bool TextField::insertChar(int c)
{
	if (curLen >= maxLen)
		return false;

	//verify character is within ascii range
	if (c < ' ' || c > '~')
		return false;

	short moveSize = maxLen - cursorPos - 1;
	memcpy_s(textPtr + sizeof(char), moveSize, textPtr, moveSize);

	*textPtr = c;

	shiftCursor(1);

	curLen++;
	return true;
}


bool TextField::inputChar(int c)
{
	short moveSize;
	bool result = true;
	switch (c)
	{
	case '\b':
		if (cursorPos <= 0)
			return false;
		//backup one character and then use regular delete character routine
		shiftCursor(-1);
		result = deleteChar();
		break;
	case KEY_DC:
		result = deleteChar();
		break;
	case KEY_LEFT: 
		if (cursorPos <= 0)
			return false;

		shiftCursor(-1);
		break;
	case KEY_RIGHT: 
		if (cursorPos >= curLen) //can't move right after last character
			return false;

		shiftCursor(1);
		break;
	case KEY_HOME: 
		moveHome();
		break;
	case KEY_END: 
		moveEnd();
		break;
	default: //all printable characters
		result = insertChar(c);	
		break;
	}

	return true;
}

void TextField::shiftCursor(int amount)
{
	moveCursor(cursorPos + amount);
}

void TextField::moveCursor(unsigned int position)
{
	cursorPos = position;
	textPtr = text + cursorPos;
}

void TextField::moveHome()
{
	moveCursor(0);
}

void TextField::moveEnd()
{
	moveCursor(curLen);
}


void TextField::setText(string text)
{
	clear();
	const char* t = text.c_str();
	memcpy_s(this->text, maxLen, t, text.length());
	curLen = text.length();
	cursorPos = curLen;
	textPtr = this->text + cursorPos;
}

void TextField::setText(int value)
{
	ostringstream oss;
	oss << value;
	setText(oss.str());
}

string TextField::getText()
{
	string t(text, curLen);
	
	//trim algorithm
	t.erase(0, t.find_first_not_of(' '));       //prefixing spaces
	t.erase(t.find_last_not_of(' ') + 1);         //suffixing spaces
	return t;
}

WINDOW* TextField::getWindow()
{
	return win;
}

void TextField::draw()
{
	wclear(win);
	wbkgd(win, color);
	
	char* pos = text;
	bool endReached = false;

	for (int col = 0; col < visibleCols && col < maxLen; col++)
	{
		mvwaddch(win, 0, col, *pos++);
	}


	wnoutrefresh(win);
}


void TextField::setColor(int bkgdColor, int textColor)
{
	color = bkgdColor << 28 | textColor << 24;
	color &= 0xff000000;
}


void TextField::clear()
{
	memset(text, BLANK_SPACE, curLen);
	moveHome();
	curLen = 0;
	
	/*textPtr = text;
	cursorPos = 0;*/
}

TextField::~TextField()
{
	delete text;
	delwin(win);
}