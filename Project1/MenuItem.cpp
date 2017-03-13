#include "MenuItem.h"

MenuItem::MenuItem()
{
	clear();
}

void MenuItem::clear()
{
	selectable = false;
//	selected = false;
	crossref = -1;
	index = -1;
}

