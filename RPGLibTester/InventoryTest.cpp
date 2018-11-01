#include "CppUnitTest.h"
#include "Inventory.h"
#include "GridMenu.h"
#include "LineItem.h"
#include "OwnedItemRecord.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace RPGLibTester
{
	TEST_CLASS(InventoryTest)
	{
		TEST_METHOD(constructorTest)
		{
			Inventory inv(1, 1);
			Assert::AreEqual(1, (int)inv.getSize());
		}

		TEST_METHOD(addItemToInventory)
		{
			Inventory inv(1,1);

			GameItem* cig = new GameItem;
			cig->name = "Cigarette";

			inv.addItem(cig);

			Assert::IsTrue(inv.getItemAtIndex(0)->name.compare(cig->name) == 0);
			delete cig;
		}

		TEST_METHOD(discardItemTest)
		{
			Inventory inv(1, 1);

			GameItem* cig = new GameItem;
			cig->name = "Cigarette";

			inv.addItem(cig);

			inv.discardItem(0);

			Assert::AreEqual(0, (int)inv.getItemCount());
			delete cig;
		}
		
		TEST_METHOD(addItemToInventoryFirstBlankSpot)
		{
			Inventory inv(3, 1);

			GameItem* item1 = new GameItem;
			item1->name = "Potion";

			GameItem* item2 = new GameItem;
			item2->name = "Memo";

			GameItem* item3 = new GameItem;
			item3->name = "Sword";

			inv.addItem(item1);
			inv.addItem(item2);
			inv.addItem(item3);

			GameItem* item4 = new GameItem;
			std::string item4Name = "Meat";
			item4->name = item4Name;

			inv.discardItem(1); //will delete item2

			inv.addItem(item4); //should add in item2's place

			Assert::IsTrue(inv.getItemAtIndex(1)->name.compare(item4Name) == 0);
			delete item1;
			delete item2;
			delete item3;
			delete item4;
		}

		/*TEST_METHOD(swap2Items)
		{
			Inventory inv(2, 1);

			GameItem* item1 = new GameItem;
			std::string item1Name = "Potion";
			item1->name = item1Name;

			GameItem* item2 = new GameItem;
			item2->name = "Memo";

			inv.addItem(item1);
			inv.addItem(item2);
			
			
			inv.swapItems(0, 1);

			Assert::IsTrue(inv.getItemAtIndex(1)->name.compare(item1Name) == 0);
			delete item1;
			delete item2;
		}*/
		
	};
}