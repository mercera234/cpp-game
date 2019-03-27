#include <sstream>
#include "EquippedItem.h"
#include "game_strings.h"


EquippedItem::EquippedItem()
{
	init();
}


EquippedItem::EquippedItem(EquipPart partIn, GameItem* itemIn, int element)
{
	item = itemIn;
	part = partIn;
	index = element;
	init();
}

void EquippedItem::init()
{
	selectable = true;
	selected = false;
}

void EquippedItem::draw()
{
	MenuItem::draw();

	//draw item details
	if (part == EquipPart::BLANK)
		return;

	if (selectable == false)
	{
		wattron(win, setTextColor(COLOR_GRAY) | A_DIM);
	}

	std::ostringstream oss;

	int partNameLength = 6;  //we cut the equip part names short so they are all even
	std::string partName = toString(part).substr(0, partNameLength);

	oss << partName;
	if (partName.length() < partNameLength)
	{
		oss << std::string(partNameLength - partName.length(), ' ');
	}
	oss << ' '; //separator
	
	std::string itemName = item == nullptr ? "Nothing" : item->name;
	oss << itemName;


	mvwaddstr(win, topRow, itemX, oss.str().c_str());


	if (selectable == false)
	{
		wattroff(win, setTextColor(COLOR_GRAY) | A_DIM);
	}
}
