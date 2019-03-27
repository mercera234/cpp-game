#include "CppUnitTest.h"
#include "ItemBrowser.h"
#include "GameItem.h"
#include "Actor.h"
#include "actor_helper.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace GameLibTester
{
	TEST_CLASS(ItemBrowserTest)
	{
		TUI tui;
		ItemBrowser browser;
		Inventory inv;
		GameItem potion;
		GameItem knife;

		TEST_METHOD_INITIALIZE(start)
		{
			//create 2 items
			potion.name = "Potion";
			potion.description = "Restores 50 hp";
			
			knife.name = "Knife";
			knife.description = "A weapon for stabbing";

			browser.setInventory(&inv);
		}

		TEST_METHOD(ctorTest)
		{
			Assert::AreEqual((int)nullptr, (int)browser.getCurrentItem());
			Assert::AreEqual(0, browser.getItemCount());
		}

		TEST_METHOD(inputTest)
		{
			browser.acquireItem(&knife, 1);
			browser.acquireItem(&potion, 1);

			::processInput(browser, KEY_DOWN);

			Assert::AreEqual((int)&potion, (int)browser.getCurrentItem());
		}

		TEST_METHOD(drawTest)
		{
			browser.acquireItem(&potion, 1);

			WINDOW* aWin = TUI::winMgr.newWin(2, 10, 1, 1);
			browser.setWindow(aWin);
			browser.draw();

			char text[20];
			mvwinnstr(aWin, 0, 0, text, 4);

			Assert::AreEqual("->Po", text);
		}

		TEST_METHOD(decrementItemTest)
		{
			browser.acquireItem(&potion, 1);

			browser.decrementItem();

			Assert::AreEqual(0, browser.getItemCount()); //the item was removed from the items list
		}

		TEST_METHOD(acquireFreshItemTest)
		{
			GameItem potion;
			potion.name = "Potion";

			browser.acquireItem(&potion, 1);
			Assert::AreEqual(1, browser.getItemCount()); //only item in inventory
		}

		TEST_METHOD(acquireItemAddTest)
		{
			GameItem potion;
			potion.name = "Potion";

			browser.acquireItem(&potion, 4);
			browser.acquireItem(&potion, 3);

			Assert::AreEqual(1, browser.getItemCount()); //only item in inventory
			Assert::AreEqual(7, browser.getItemQuantity(&potion)); //only quantity of item
		}
	
	};
}