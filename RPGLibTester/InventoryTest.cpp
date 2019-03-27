#include "CppUnitTest.h"
#include "Inventory.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace RPGLibTester
{
	TEST_CLASS(InventoryTest)
	{
		Inventory inventory;
		GameItem potion;
		GameItem knife;

		TEST_METHOD_INITIALIZE(start)
		{
			//create 2 items
			potion.name = "Potion";
			potion.description = "Restores 50 hp";

			knife.name = "Knife";
			knife.description = "A weapon for stabbing";
		}

		TEST_METHOD(ctorTest)
		{
			Assert::AreEqual(0, inventory.getItemCount());
		}

		TEST_METHOD(addOneItemTest)
		{
			inventory.alterItemQuantity(&potion, 1);
			Assert::AreEqual(1, inventory.getItemCount());
		}

		TEST_METHOD(addMultipleItemTest)
		{
			inventory.alterItemQuantity(&potion, 1);
			inventory.alterItemQuantity(&knife, 1);
			Assert::AreEqual(2, inventory.getItemCount());
		}

		TEST_METHOD(addToExistingItemTest)
		{
			inventory.alterItemQuantity(&potion, 6);
			inventory.alterItemQuantity(&potion, 4);
			Assert::AreEqual(10, inventory.getItemQuantity(&potion));
		}

		TEST_METHOD(addOverflowTest)
		{
			inventory.alterItemQuantity(&potion, 99);
			Assert::IsNull(inventory.alterItemQuantity(&potion, 1));
		}

		TEST_METHOD(removeOneItemTest)
		{
			inventory.alterItemQuantity(&potion, 10);
			inventory.alterItemQuantity(&potion, -1);
			Assert::AreEqual(9, inventory.getItemQuantity(&potion));
		}

		TEST_METHOD(removeOnePossessionTest)
		{
			inventory.alterItemQuantity(&potion, 5);
			inventory.alterItemQuantity(&potion, -5);
			Assert::AreEqual(0, inventory.getItemQuantity(&potion));
		}

		TEST_METHOD(removeUnderflowTest)
		{
			inventory.alterItemQuantity(&potion, 1);
			inventory.alterItemQuantity(&potion, -2);
			Assert::AreEqual(0, inventory.getItemCount());
		}
	};
}