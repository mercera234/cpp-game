#include "CppUnitTest.h"
#include "ItemBrowser.h"
#include "GameItem.h"
#include "GameInput.h"
#include "Actor.h"
#include "actor_helper.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace GameLibTester
{
	TEST_CLASS(ItemBrowserTest)
	{
		TUI tui;
		ItemBrowser inventory;
		GameItem potion;
		GameItem knife;
		Possession* potionItem;
		Possession* knifeItem;

		TEST_METHOD_INITIALIZE(start)
		{
			//create 2 items
			potion.name = "Potion";
			potion.description = "Restores 50 hp";
			
			knife.name = "Knife";
			knife.description = "A weapon for stabbing";

			potionItem = new Possession;
			potionItem->item = &potion;
			potionItem->quantity.setCurr(1);
			potionItem->item->type = GameItemType::USABLE;

			knifeItem = new Possession;
			knifeItem->item = &knife;
			knifeItem->quantity.setCurr(1);
			knifeItem->item->type = GameItemType::EQUIPPABLE;
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
			items.push_back(potionItem);
			items.push_back(knifeItem);
			inventory.setItems(items);

			Assert::AreEqual((int)&potion, (int)inventory.getCurrentItem());
		}

		TEST_METHOD(inputTest)
		{
			std::vector<Possession*> items;
			items.push_back(knifeItem);
			items.push_back(potionItem);
			inventory.setItems(items);

			inventory.processInput(GameInput::DOWN_INPUT);

			Assert::AreEqual((int)&potion, (int)inventory.getCurrentItem());
		}

		TEST_METHOD(drawTest)
		{
			std::vector<Possession*> items;
			items.push_back(potionItem);
			inventory.setItems(items);

			WINDOW* aWin = TUI::winMgr.newWin(2, 10, 1, 1);
			inventory.setWindow(aWin);
			inventory.draw();

			char text[20];
			mvwinnstr(aWin, 0, 0, text, 4);

			Assert::AreEqual("->Po", text);
		}

		TEST_METHOD(decrementItemTest)
		{
			std::vector<Possession*> items;
			items.push_back(potionItem);
			inventory.setItems(items);

			inventory.decrementItem();

			Assert::AreEqual(0, (int)items.size()); //the item was removed from the items list
		}


	
	};
}