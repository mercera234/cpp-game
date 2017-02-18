#include "TextField.h"
#include <sstream>

TextField::TextField(WINDOW* win)
{
	focusable = true;

	this->win = win;
	getbegyx(win, y, x);
	
	fullLength = getmaxx(win) + 1;
	length = fullLength - 1;

	text = new char[length];

	memset(text, BLANK_SPACE, length);
	textPtr = text;
	text[length] = 0; //end with null term
	cursorPos = 0; //starts at beginning
}

void TextField::setFocus()
{
	curs_set(1);
	move(y, x + cursorPos); //a known issue with using wmove combined with getch is why we're using move
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
	t.erase(t.find_last_not_of(' ') + 1);         //surfixing spaces
	return t;
}

WINDOW* TextField::getWindow()
{
	return win;
}

void TextField::draw()
{
	wclear(win);
	mvwaddstr(win, 0, 0, text);
	wnoutrefresh(win);
}

TextField::~TextField()
{
	delete text;
	delwin(win);
}