#include "CppUnitTest.h"
#include "Palette.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PDCurseControlsTester
{
	TEST_CLASS(PaletteTest)
	{
		TUI tui;

		TEST_METHOD_INITIALIZE(startTUI)
		{
			tui.init();
		}

		TEST_METHOD_CLEANUP(stopTUI)
		{
			tui.shutdown();
		}

		TEST_METHOD(setTitleTest)
		{
			Palette p;

			std::string title = "Here is title";
			p.setTitle(title);

			Assert::AreEqual(title, p.getTitle());
		}

		TEST_METHOD(setWindowsTest)
		{
			Palette p;

			int rows = 1;
			p.setWindows(0, 0, 1, 1);

			int paletteHeight = getmaxy(p.getWindow());
			Assert::AreEqual(rows + 2, paletteHeight); //2 rows more than supplied
		}


		TEST_METHOD(paletteParameterConstructorTest)
		{
			Palette p(1,1, "A",1,1);
			Assert::AreEqual(*(new std::string("A")), p.getTitle()); //don't get used to this method of passing strings
		}


		TEST_METHOD(setItemTest)
		{
			Palette p(3, 4, "B", 2, 2);
			p.setItem("The item", 'R', 0);

			LineItem* item = p.getItem(0, 0);
			Assert::AreEqual('R', (char)item->getIcon());
		}

	/*	TEST_METHOD(setFocusTest)
		{
			Palette p(1, 1, "B", 1, 1);
			
			p.setFocus(true);

			

			LineItem* item = p.getItem(0, 0);
			Assert::AreEqual('R', (char)item->getIcon());
		}*/
	};
}