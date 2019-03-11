#include "OwnedItemRecord.h"

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
	if (possession->item == nullptr)
		return;

	if (selectable == false)
	{
		wattron(win, setTextColor(COLOR_GRAY) | A_DIM);
	}

	mvwaddstr(win, topRow, itemX, possession->item->name.c_str());

	if (selectable == false)
	{
		wattroff(win, setTextColor(COLOR_GRAY) | A_DIM);
	}
}