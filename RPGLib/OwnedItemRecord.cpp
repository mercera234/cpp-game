#include "OwnedItemRecord.h"
#include <sstream>

OwnedItemRecord::OwnedItemRecord()
{
	init();
}


OwnedItemRecord::OwnedItemRecord(Possession* possessionIn, int element)
{
	possession = possessionIn;
	index = element;
	init();
}

void OwnedItemRecord::init()
{
	selectable = true;
	selected = false;
}

void OwnedItemRecord::draw()
{
	MenuItem::draw();

	//draw item details
	if (possession == nullptr || possession->item == nullptr)
		return;

	if (selectable == false)
	{
		wattron(win, setTextColor(COLOR_GRAY) | A_DIM);
	}

	//needs to draw the item quantities as well
	mvwaddstr(win, topRow, itemX, possession->item->name.c_str());

	int winWidth = getmaxx(win);

	std::ostringstream oss;
	oss << possession->quantity.getCurr();
	mvwaddstr(win, topRow, winWidth - 2, oss.str().c_str());


	if (selectable == false)
	{
		wattroff(win, setTextColor(COLOR_GRAY) | A_DIM);
	}
}