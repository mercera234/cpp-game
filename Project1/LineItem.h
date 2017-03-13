#pragma once
#include "MenuItem.h"

class LineItem : public MenuItem
{
private:
public:
	string name;
	LineItem(string name, int element, int crossRefNdx);
	void draw(WINDOW* win, int y, int x);
	void clear();
};