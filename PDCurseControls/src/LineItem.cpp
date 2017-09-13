#include "LineItem.h"
#include "AbstractMenu.h"

LineItem::LineItem(std::string name, int element, int crossRefNdx)
{
	index = element;
	crossRef = crossRefNdx;
	this->name = name;
	selectable = true;
	selected = false;
}

//void LineItem::draw(WINDOW* win, int offY, int offX)
//{
//	mvwaddstr(win, posY + offY, posX + offX, name.c_str()); //get item name
//}

void LineItem::draw()
{
	int offY = menu->getUlY();
	int offX = menu->getUlX();

	int markPosX = 0;
	int itemPosX = 0;
	//for now, our main focus will be on getting the left mark working
	/*switch (menu->getMarkSide())
	{
	case LEFT_MARK: */
		markPosX = posX - offX;
		itemPosX = markPosX + 2;
	//	break;
	//case RIGHT_MARK: 
	//	itemPosX = posX - offX;
	//	markPosX = itemPosX + name.length(); //should maybe use width of the item, but this is not an abstract property currently
	//	break;
	//}

	
	if (this == menu->getCurrentItem()) //draw mark
	{
		mvwaddstr(menu->getWindow(), posY - offY, markPosX, menu->getCursor().c_str()); //get item name
	}
	//draw item
	mvwaddstr(menu->getWindow(), posY - offY, itemPosX, name.c_str()); //get item name
	
}

void LineItem::clear()
{
	name = "";
	MenuItem::clear();
}