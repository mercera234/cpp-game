#include "CppUnitTest.h"
#include "MockMenu.h"
#include "MockMenuItem.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PDCurseControlsTester
{
	TEST_CLASS(AbstractMenuTest)
	{
		TEST_METHOD(setMaxCapacityTest)
		{
			MockMenu menu;

			int value = 10;
			menu.setMaxCapacity(value);

			Assert::AreEqual(value, (int)menu.getMaxCapacity());
		}

		/*
		Changing the menu to a lower capacity will not cause a reallocation
		*/
		TEST_METHOD(setMaxCapacityLessThanCurrentCapacityTest)
		{
			MockMenu menu;

			int value = 16;
			menu.setMaxCapacity(value);
			int newValue = 9;
			menu.setMaxCapacity(newValue);

			Assert::AreEqual(value, (int)menu.getMaxCapacity()); //capacity should equal original value
		}

		TEST_METHOD(setMaxItemsTest)
		{
			MockMenu menu;

			int value = 8;
			menu.setMaxItems(value);

			Assert::AreEqual(value, (int)menu.getMaxItems());
		}

		/*
		Changing max items to lower value doesn't affect capacity
		*/
		TEST_METHOD(setMaxItemsCapacityCheckTest)
		{
			MockMenu menu;

			int value = 15;
			menu.setMaxItems(value);
			int newValue = 6;
			menu.setMaxItems(newValue);

			Assert::AreEqual(value, (int)menu.getMaxCapacity());
		}

		TEST_METHOD(changeMaxItemsToLowerValueTest)
		{
			MockMenu menu;

			int value = 3;
			menu.setMaxItems(value);
			MockMenuItem* mmi = new MockMenuItem();
			mmi->index = 2;
			menu.setItem(mmi);

			int newValue = 2;
			menu.setMaxItems(newValue); //should delete all items >= newValue

			Assert::AreNotEqual(2, (int)mmi->index);  //mmi will be pointing to garbage data now
		}

		TEST_METHOD(setItemTest)
		{
			MockMenu menu;
			menu.setMaxItems(1);

			MockMenuItem* mmi = new MockMenuItem();
			int index = 0;
			mmi->index = index;
			
			menu.setItem(mmi);
			menu.setCurrentItem(index);

			MenuItem* mmi2 = menu.getCurrentItem();
			Assert::AreEqual(index, (int)mmi2->index);
		}

		TEST_METHOD(setNoCurrentItemTest)
		{
			MockMenu menu;
			
			MenuItem* mmi = menu.getCurrentItem();
			Assert::IsTrue(mmi == nullptr);
		}

		TEST_METHOD(overwriteAfterSetItemTest)
		{
			MockMenu menu;
			menu.setMaxItems(1);

			MockMenuItem* mmi = new MockMenuItem();
			int index = 0;
			mmi->index = index;

			menu.setItem(mmi);

			MockMenuItem* mmi2 = new MockMenuItem();
			mmi2->index = index;
			menu.setItem(mmi2);

			Assert::IsTrue(menu.getItem(index) == mmi2);
		}

		TEST_METHOD(clearItemTest)
		{
			MockMenu menu;
			menu.setMaxItems(1);

			MockMenuItem* mmi = new MockMenuItem();
			int index = 0;
			mmi->index = index;

			menu.setItem(mmi);
			menu.clearItem(index);

			Assert::IsNull(menu.getCurrentItem());
		}

		TEST_METHOD(clearItemsTest)
		{
			MockMenu menu;
			menu.setMaxItems(3);

			MockMenuItem* mmi = new MockMenuItem();
			mmi->index = 0;
			MockMenuItem* mmi2 = new MockMenuItem();
			mmi2->index = 2;

			menu.setItem(mmi);
			menu.setItem(mmi2);

			menu.clearItems();

			MenuItem* mi = menu.getItem(2);
			Assert::IsNull(mi);
		}

		TEST_METHOD(setSelectedTest)
		{
			MockMenu menu;
			menu.setMaxItems(1);

			MockMenuItem* mmi = new MockMenuItem();
			mmi->index = 0;
			
			menu.setItem(mmi);
			menu.setSelected(0, true);
			
			MenuItem* mi = menu.getItem(0);
			Assert::IsTrue(mi->selected);
		}

		
	};
}