#pragma once
#include "Controllable.h"
#include "curses.h"
#include <iostream>
using namespace std;

#define BLANK_SPACE ' '
class TextField : public Controllable
{
private:
	char* text;
	unsigned short fullLength; //length of string plus one for null term
	unsigned short length;

	//these 2 vars must be adjusted in tandem
	char* textPtr;
	unsigned short cursorPos;
	
//	WINDOW* win;
	short y, x; //beg y and x of window. Used for the move routine due to the issue with wmove


public:
	TextField(WINDOW* win);
	void setFocus();
	bool inputChar(int c);
	void setText(string text);
	void setText(int value);
	string getText();
	WINDOW* getWindow();
	void draw();

	~TextField();
};