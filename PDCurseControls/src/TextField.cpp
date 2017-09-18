#include <sstream>
#include <cctype>
#include <algorithm>
#include "TextField.h"
#include "TUI.h"

TextField::TextField()
{
	init();
}


TextField::TextField(unsigned int length, unsigned short y, unsigned short x)
{
	init();

	setupField(length, y, x);
}

void TextField::setupField(unsigned int length, unsigned short y, unsigned short x)
{
	win = newwin(1, length, y, x);//height is always 1 for text fields

	setWindow(win);
	getbegyx(win, scrY, scrX);

	int winLength = getmaxx(win);
	text.reserve(winLength);
	setDimensions(1, winLength);
}

void TextField::init()
{
	focusable = true;
	acceptsMouseInput = false;
	setEditable(true);

	cursorPos = 0; //starts at beginning
}


void TextField::setCursorFocus()
{
	curs_set(CURSOR_NORMAL);

	//apply move globally to screen
	//need to use :: to differentiate this from the Controllable move method
	::move(scrY, scrX + cursorPos); //a known issue with using wmove combined with getch is why we're using move
}

bool TextField::deleteChar()
{
	if (cursorPos >= text.length())
		return false;

	auto it = text.begin() + cursorPos;
	text.erase(it);

	return true;
}


bool TextField::insertChar(int c)
{
	if (text.length() >= text.capacity()) //don't exceed the set capacity
		return false;

	//verify character is within ascii range
	if (isprint(c) == false)
		return false;

	std::string::iterator it = text.begin() + cursorPos;
	text.insert(it, c);

	cursorPos++;
	return true;
}


bool TextField::inputChar(int c)
{
	bool result = true;
	switch (c)
	{
	case '\b': //backspace
		if (cursorPos <= 0)
			return false;
		//backup one character and then use regular delete character routine
		cursorPos--;
		result = deleteChar();
		break;
	case KEY_DC:
		result = deleteChar();
		break;
	case KEY_LEFT: 
		if (cursorPos <= 0)
			return false;

		cursorPos--;
		break;
	case KEY_RIGHT: 
		if (cursorPos >= text.length()) //can't move right after last character
			return false;

		cursorPos++;
		break;
	case KEY_HOME: 
		cursorPos = 0;
		break;
	case KEY_END: 
		cursorPos = text.length();
		break;
	case KEY_MOUSE: //does nothing right now
		break;
	default: //all printable characters
		result = insertChar(c);	
		break;
	}

	return result;
}


void TextField::setText(const std::string& textIn)
{
	clear();

	text.assign(textIn, 0, text.capacity());

	cursorPos = textIn.length();
}

void TextField::setText(int value)
{
	std::ostringstream oss; //convert to string first then called overloaded method
	oss << value;
	setText(oss.str());
}

std::string TextField::getText()
{
	std::string retText = text;
	retText.erase(0, retText.find_first_not_of(' '));       //prefixing spaces
	retText.erase(retText.find_last_not_of(' ') + 1);

	return retText;
}


void TextField::draw()
{
	wclear(win);
	//wbkgd(win, color);
	
	mvwaddstr(win, 0, 0, text.c_str());
	wnoutrefresh(win);
}


void TextField::clear()
{
	text.clear();
	cursorPos = 0;
}

TextField::~TextField()
{
	if(win != nullptr)
		delwin(win);
}