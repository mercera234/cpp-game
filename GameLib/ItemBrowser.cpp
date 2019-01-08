#include "ItemBrowser.h"
#include "DialogWindow.h"
#include "GridMenu.h"
#include "LineItem.h"
#include "GameInput.h"
#include "ExitCode.h"
#include "menu_drivers.h"
#include "GameItem.h"
#include "OwnedItemRecord.h"
#include "TextLabel.h"


ItemBrowser::ItemBrowser()
{ 
	menu.setWrapAround(false); 
}

void ItemBrowser::setWindow(WINDOW* win)
{
	menu.setWindow(win);
}


void ItemBrowser::processInput(int input)
{
	menuDriver(input, &menu);
}


void ItemBrowser::setItems(std::vector<OwnedItem*>& items)
{
	inventoryPtr = &items; //save reference to items

	menu.resetItems(items.size(), 1);

	int element = 0;
	for each (OwnedItem* item in items)
	{
		menu.setItem(new OwnedItemRecord(item, element++));
	}
	
	menu.setCurrentItem(0);
	menu.post(true);
}

void ItemBrowser::draw()
{
	menu.draw();
}

OwnedItemRecord* ItemBrowser::getCurrentItem()
{
	return (OwnedItemRecord*)menu.getCurrentItem();
}
