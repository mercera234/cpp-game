#pragma once
#include "AbstractMenu.h"

class GraphMenu : public AbstractMenu
{
private:

	void dirDriver(int input);
	
public:
	GraphMenu(WINDOW* win, int capacity);
	//void setItem(MenuItem* item);
	void draw();
	//void clear();
	//void selectItem(int index);
	int driver(int input);
};