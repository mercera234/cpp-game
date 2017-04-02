#pragma once
#include "MenuItem.h"

struct LineItem : public MenuItem
{
	string name;
	LineItem(string name, int element, int crossRefNdx);
	void draw(WINDOW* win, int offY, int offX);
	void draw(WINDOW* win);
	void clear();
};