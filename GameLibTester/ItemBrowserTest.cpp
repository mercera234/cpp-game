#include "CppUnitTest.h"
#include "ItemBrowser.h"
#include "GameItem.h"
#include "GameInput.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace GameLibTester
{
	TEST_CLASS(ItemBrowserTest)
	{
		TUI tui;
		ItemBrowser inventory;
		GameItem potion;
		GameItem knife;
		Possession item;
		Possession item2;

		TEST_METHOD_INITIALIZE(start)
		{
			//create 2 items
			potion.name = "Potion";
			potion.description = "Restores 50 hp";
			
			knife.name = "Knife";
			knife.description = "A weapon for stabbing";

			item.item = &potion;
			item.quantity.setCurr(1);
			item.item->type = GameItemType::USABLE;

			item2.item = &knife;
			item2.quantity.setCurr(1);
			item2.item->type = GameItemType::EQUIPPABLE;
		}

		TEST_METHOD(ctorTest)
		{
			Assert::AreEqual((int)nullptr, (int)inventory.getCurrentItem());
		}

		TEST_METHOD(setItemsNoneTest)
		{
			std::vector<Possession*> items;
			inventory.setItems(items);

			Assert::AreEqual((int)nullptr, (int)inventory.getCurrentItem());
		}

		TEST_METHOD(setItemsTest)
		{
			std::vector<Possession*> items;
			items.push_back(&item);
			items.push_back(&item2);
			inventory.setItems(items);

			Assert::AreEqual((int)&item, (int)inventory.getCurrentItem()->getPossession());
		}

		TEST_METHOD(inputTest)
		{
			std::vector<Possession*> items;
			items.push_back(&item2);
			items.push_back(&item);
			inventory.setItems(items);

			inventory.processInput(GameInput::DOWN_INPUT);

			Assert::AreEqual((int)&item, (int)inventory.getCurrentItem()->getPossession());
		}

		TEST_METHOD(drawTest)
		{
			std::vector<Possession*> items;
			items.push_back(&item);
			inventory.setItems(items);

			WINDOW* aWin = TUI::winMgr.newWin(2, 10, 1, 1);
			inventory.setWindow(aWin);
			inventory.draw();

			char text[20];
			mvwinnstr(aWin, 0, 0, text, 4);

			Assert::AreEqual("->Po", text);
		}

		//TEST_METHOD(refreshTest)
		//{
		//	std::vector<Possession*> items;
		//	item.quantity.setCurr(0);
		//	items.push_back(&item);
		//	items.push_back(&item2);
		//	inventory.setItems(items);

		//	inventory.draw(); //drawing should refresh the list and will delete the empty item
		//	Assert::AreEqual(1, (int)items.size());
		//}
	};
}