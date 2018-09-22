#include "inventory_tests.h"
#include "Inventory.h"

/*Works, but I'm not sure I like how I am referencing both the inventory and the gridmenu separately*/
void inventoryTest() 
{
	resize_term(23, 51);

	GameItem* item1 = new GameItem;
	item1->name = "Cell Water";
	item1->cost = 10;
	item1->description = "Water for hydration";
	item1->type = GameItemType::CONSUMABLE;
	
	GameItem* item2 = new GameItem;
	item2->name = "Tazer";
	item2->cost = 500;
	item2->description = "Stuns enemies";
	item2->type = GameItemType::EQUIPPABLE;

	GameItem* item3 = new GameItem;
	item3->name = "Laser Sword";
	item3->cost = 10000;
	item3->description = "Weapon";
	item3->type = GameItemType::EQUIPPABLE;

	Inventory inv(10, 1);
	GridMenu* menu = inv.getGridMenu();

	menu->setWindow(stdscr);
	menu->setFocus(true);
	menu->post(true);

	bool usingMenu = true;
	while (usingMenu)
	{
		menu->draw();
		doupdate();
		int input = getch();

		switch (input)
		{
		case KEY_ESC: usingMenu = false; break;
		case '1': inv.addItem(item1); break;
		case '2': inv.addItem(item2); break;
		case '3': inv.addItem(item3); break;
		case 'd': 
		{
			inv.discardItem(menu->getCurrentItem()->index);
		} break;
		/*case '\r':
		case '\n':*/
		//case KEY_ENTER:
		//{
		//	MenuItem* item = AbstractMenu::basicMenuDriver(input, &itemMenu);

		//	if (item == nullptr)
		//	{
		//		continue;
		//	}

		//	//switch (item->index)
		//	//{
		//	//default: //set up swap ability here
		//	//	if (swapState == false)
		//	//	{
		//	//		item->selected = true;
		//	//		swapIndex = item->index;
		//	//		swapState = true;
		//	//	}
		//	//	else
		//	//	{
		//	//		unsigned short origIndex = item->index;
		//	//		itemMenu.swapItems(item->index, swapIndex);
		//	//		itemMenu.setCurrentItem(origIndex);
		//	//		swapState = false;

		//	//		MenuItem* item2 = itemMenu.AbstractMenu::getItem(origIndex);
		//	//		item2->selected = false;
		//	//	}

		//	//	break;
		//	//}
		//}
		//break;
		default:
			AbstractMenu::basicMenuDriver(input, menu);
			break;
		}
	}
}