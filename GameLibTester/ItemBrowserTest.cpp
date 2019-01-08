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
		OwnedItem item;
		OwnedItem item2;

		TEST_METHOD_INITIALIZE(start)
		{
			//create 2 items
			potion.name = "Potion";
			potion.description = "Restores 50 hp";
			
			knife.name = "Knife";
			knife.description = "A weapon for stabbing";

			item.item = &potion;
			item.quantity = 1;

			item2.item = &knife;
			item2.quantity = 1;
		}

		TEST_METHOD(ctorTest)
		{
			Assert::AreEqual((int)nullptr, (int)inventory.getCurrentItem());
		}

		TEST_METHOD(setItemsNoneTest)
		{
			std::vector<OwnedItem*> items;
			inventory.setItems(items);

			Assert::AreEqual((int)nullptr, (int)inventory.getCurrentItem());
		}

		TEST_METHOD(setItemsTest)
		{
			std::vector<OwnedItem*> items;
			items.push_back(&item);
			items.push_back(&item2);
			inventory.setItems(items);

			Assert::AreEqual((int)&item, (int)inventory.getCurrentItem()->getPossession());
		}

		TEST_METHOD(inputTest)
		{
			std::vector<OwnedItem*> items;
			items.push_back(&item);
			items.push_back(&item2);
			inventory.setItems(items);

			inventory.processInput(GameInput::DOWN_INPUT);

			Assert::AreEqual((int)&item2, (int)inventory.getCurrentItem()->getPossession());
		}

		TEST_METHOD(drawTest)
		{
			std::vector<OwnedItem*> items;
			items.push_back(&item);
			inventory.setItems(items);

			WINDOW* aWin = TUI::winMgr.newWin(2, 10, 1, 1);
			inventory.setWindow(aWin);
			inventory.draw();

			char text[20];
			mvwinnstr(aWin, 0, 0, text, 4);

			Assert::AreEqual("->Po", text);
		}
	};
}