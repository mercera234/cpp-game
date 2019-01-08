#pragma once
#include "GridMenu.h"

class MenuBuilder
{
public:
	MenuBuilder() {}
	
	void buildTitleMenu(GridMenu& menu, WINDOW* win, int titleSpace);

};

