#include "CppUnitTest.h"
#include "GridMenu.h"
#include "MockMenuItem.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PDCurseControlsTester
{
	TEST_CLASS(GridMenuTest)
	{
		TUI tui;
		WINDOW* stdscrCpy = nullptr;
		TEST_METHOD_INITIALIZE(start)
		{
			stdscrCpy = dupwin(stdscr);
		}

		TEST_METHOD_CLEANUP(end)
		{
			delwin(stdscrCpy);
		}

		TEST_METHOD(constructorTest)
		{
			int rows = 3;
			int cols = 7;
			
			GridMenu menu(stdscrCpy, rows, cols);

			Assert::AreEqual(rows * cols, (int)menu.getMaxItems());
		}

		TEST_METHOD(resetItemsTest)
		{
			int rows = 1;
			int cols = 1;
			GridMenu menu(stdscrCpy, rows, cols);

			MockMenuItem* mmi = new MockMenuItem();
			mmi->index = 0;
			menu.setItem(mmi);

			menu.resetItems(1,2);
			Assert::IsNull(menu.getItem(0,0));
		}

		TEST_METHOD(setItemTest)
		{
			int rows = 1;
			int cols = 2;
			GridMenu menu(stdscrCpy, rows, cols);

			MockMenuItem* mmi = new MockMenuItem();
			mmi->index = 0;
			menu.setItem(mmi, 0, 1);

			Assert::AreEqual(1, (int)menu.getItem(0, 1)->index); //using set Item with row col params changes index of item
		}

		TEST_METHOD(setItemNoArgsTest)
		{
			int rows = 1;
			int cols = 2;
			GridMenu menu(stdscrCpy, rows, cols);

			MockMenuItem* mmi = new MockMenuItem();
			mmi->index = 1;
			menu.setItem(mmi);

			Assert::AreEqual(1, (int)menu.getItem(0, 1)->index); 
		}

		TEST_METHOD(postTest)
		{
			int rows = 1;
			int cols = 2;
			GridMenu menu(stdscrCpy, rows, cols);

			MockMenuItem* mmi = new MockMenuItem();
			menu.setItem(mmi, 0, 0);

			MockMenuItem* mmi2 = new MockMenuItem();
			menu.setItem(mmi2, 0, 1);
			menu.setWrapAround(true); //true by default, but setting explicitly here for test
			menu.post(true);

			//wrap around links are correct
			Assert::IsTrue(mmi->getLinkedItem(Dir::LEFT) == mmi2);
			Assert::IsTrue(mmi->getLinkedItem(Dir::UP) == mmi);
		}

		TEST_METHOD(cantPostTest)
		{
			int rows = 3;
			int cols = 1;
			GridMenu menu(stdscrCpy, rows, cols);

			MockMenuItem* mmi = new MockMenuItem();
			menu.setItem(mmi, 0, 0);

			MockMenuItem* mmi2 = new MockMenuItem();
			menu.setItem(mmi2, 1, 0);

			menu.setWrapAround(true); //true by default, but setting explicitly here for test
			menu.post(true); //posting without a third item added

			Assert::IsFalse(menu.isPosted()); //menu will not be posted
			Assert::IsFalse(mmi->getLinkedItem(Dir::DOWN) == mmi2); //items 1 and 2 will not be linked
		}

		TEST_METHOD(postEmptyTest)
		{
			int rows = 0;//if either of these is 0 then the menu can't hold any items
			int cols = 1;
			GridMenu menu(stdscrCpy, rows, cols);

			menu.post(true);

			//wrap around links are correct
			Assert::IsFalse(menu.isPosted());
		}

		

		


		/*TEST_METHOD(swapItemsTest)
		{
			MockMenu menu;
			menu.setMaxItems(2);

			MockMenuItem* mmi = new MockMenuItem();
			mmi->index = 0;
			int firstCrossRef = 35;
			mmi->crossRef = firstCrossRef;
			menu.setItem(mmi);

			MockMenuItem* mmi2 = new MockMenuItem();
			mmi2->index = 1;
			int secondCrossRef = 12;
			mmi2->crossRef = secondCrossRef;
			menu.setItem(mmi2);

			mmi->link(Dir::RIGHT, mmi2);
			menu.swapItems(0, 1);

			Assert::AreEqual(secondCrossRef, menu.getItem(0)->crossRef);
			Assert::IsTrue(menu.getItem(0)->getLinkedItem(Dir::RIGHT) == menu.getItem(1));
		}*/
	};
}