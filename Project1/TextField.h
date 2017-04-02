#pragma once
#include "Controllable.h"
#include "curses.h"
#include <iostream>
using namespace std;

struct TextStorage
{
	char* text;
	unsigned short length;

	TextStorage(int length)
	{
		this->length = length;
		text = new char[length];
	}

	/*
	addLine(string line)

	*/
};


#define BLANK_SPACE ' '
class TextField : public Controllable
{
private:
	char* text; //may want to consider the gap method of data storage at some point
	unsigned short length;

	//these 2 vars must be adjusted in tandem
	char* textPtr;
	unsigned short cursorPos;

	
	chtype color; //a mask to indicate the color
	short y, x; //beg y and x of window. Used for the move routine due to the issue with wmove
	void init(WINDOW* win);
public:
	TextField(WINDOW* win);
	TextField(int length, int y, int x);
	void setFocus();
	bool inputChar(int c);
	void setText(string text);
	void setText(int value);
	void setColor(int bkgdColor, int textColor = COLOR_WHITE);
	string getText();
	WINDOW* getWindow();
	void draw();

	~TextField();
};