#include "CppUnitTest.h"
#include "GameItem.h"
#include "Money.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace RPGLibTester
{
	TEST_CLASS(PickupTest)
	{
		TEST_METHOD(setItemProperties)
		{
			GameItem item;
			item.name = "Cigarette";
			item.description = "Soothes nerves";
			item.symbol = defaultItemSymbol;
			item.y = 0;
			item.x = 0;
			item.cost = 3;
			item.type = GameItemType::CONSUMABLE;

			Assert::IsTrue(true);
		}

		TEST_METHOD(setMoneyProperties)
		{
			Money money;
			money.name = "Gold $";
			money.symbol = defaultMoneySymbol;
			
			money.amount = 100;

			Assert::IsTrue(true);
		}

		TEST_METHOD(pickup)
		{
			GameItem potion;
			potion.name = "Potion";
			
			Pickup p = potion;

			Assert::IsTrue(p.name.compare("Potion") == 0);
		}

		/*

		Actor player;
		player.get(item); //load into Inventory

		player.use(item);

		player.setItem(item);

		money.earn(money);
		money.gain(money);
		*/
	};
}