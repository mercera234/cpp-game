#include "OwnedItemRecord.h"

OwnedItemRecord::OwnedItemRecord(Possession* possessionIn, int element)
{
	possession = possessionIn;
	index = element;
	selectable = true;
	selected = false;
}

void OwnedItemRecord::draw()
{
	MenuItem::draw();

	//draw item details
	if (possession->item == nullptr)
		return;

	mvwaddstr(win, topRow, itemX, possession->item->name.c_str());
}