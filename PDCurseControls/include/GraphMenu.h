#pragma once
#include "AbstractMenu.h"

class GraphMenu : public AbstractMenu
{
private:
	void dirDriver(int input);
	
public:
	GraphMenu(WINDOW* win, int capacity);
	void draw();
	int driver(int input);
};