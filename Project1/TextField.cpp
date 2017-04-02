#include "TextField.h"
#include <sstream>

TextField::TextField(WINDOW* win)
{
	init(win);
}

TextField::TextField(int length, int y, int x)
{
	WINDOW* w = newwin(1, length, y, x);
	init(w);
}

void TextField::init(WINDOW* win)
{
	focusable = true;

	setWindow(win);
	//this->win = win;
	getbegyx(win, y, x);

	length = getmaxx(win);
	text = new char[length + 1]; //set one more so there is a null char at the end

	memset(text, BLANK_SPACE, length);
	textPtr = text;
	text[length] = 0; //end with null term
	cursorPos = 0; //starts at beginning

	color = 0x00000000; //set to black and white by default
}

void TextField::setFocus()
{
	curs_set(1);
	//need to use :: to differentiate this from the Controllable move method
	::move(y, x + cursorPos); //a known issue with using wmove combined with getch is why we're using move
}

bool TextField::inputChar(int c)
{
	short moveSize;
	switch (c)
	{
	case '\b':
		if (cursorPos <= 0)
			return false;
		//shift string beyond position up a space
		moveSize = length - cursorPos; //length of printable characters - one beyond current position
		textPtr--;
		cursorPos--;
		memcpy_s(textPtr, moveSize, textPtr + sizeof(char), moveSize);
		//clear last position
		text[length - 1] = BLANK_SPACE;
		break;
	case KEY_DC:
		if (cursorPos >= length)
			return false;
		//shift string beyond position up a space
		moveSize = length - (cursorPos + 1); //length of printable characters - one beyond current position
		memcpy_s(textPtr, moveSize, textPtr + sizeof(char), moveSize);
		//clear last position
		text[length - 1] = BLANK_SPACE;
		break;
	case KEY_LEFT: 
		if (cursorPos <= 0)
			return false;

		cursorPos--; textPtr--; break;
	case KEY_RIGHT: 
		if (cursorPos >= length) //can't move write after last character
			return false;

		cursorPos++; textPtr++; break;

	case KEY_HOME: 
		cursorPos = 0;
		textPtr = text;
		break;
	case KEY_END: 
		cursorPos = length - 1;
		textPtr = text + length - 1;
		break;
	default: //all printable characters
		if (cursorPos >= length)
			return false;

		//verify character is within ascii range
		if (c < ' ' || c > '~')
			return false;

		moveSize = length - cursorPos - 1;
		memcpy_s(textPtr + sizeof(char), moveSize, textPtr, moveSize);

		*textPtr++ = c; 
		cursorPos++;
		
		break;
	}

	return true;
}

void TextField::setText(string text)
{
	memset(this->text, BLANK_SPACE, length);
	const char* t = text.c_str();
	memcpy_s(this->text, text.length(), t, text.length());
	cursorPos = text.length();
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
	string t = text;

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
	//wattron(win, color);

	char* pos = text;
	bool endReached = false;

	for (int row = 0; row < visibleRows && endReached == false; row++)
	{
		for (int col = 0; col < visibleCols && endReached == false; col++)
		{
			switch (*pos)
			{
			case '\n': pos++; break;//advance beyond newline
			case 0: endReached = true; break;
			default: mvwaddch(win, row, col, *pos++);
			}
		}

	}

	//add
	//mvwaddstr(win, 0, 0, text);
	wnoutrefresh(win);
}


void TextField::setColor(int bkgdColor, int textColor)
{
	color = bkgdColor << 28 | textColor << 24;
	color &= 0xff000000;
}


TextField::~TextField()
{
	delete text;
	delwin(win);
}