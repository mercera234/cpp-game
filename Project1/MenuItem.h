#pragma once

#include <iostream>
using namespace std;
#include "curses.h"

struct MenuItem
{
protected:
public:
	short index;
	int crossref;
	//bool selected;
	bool selectable;

	virtual void draw(WINDOW* win, int y, int x) = 0;
	MenuItem();
	void clear();
};

