#include "stdafx.h"
#include "CppUnitTest.h"
#include "TUI.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PDCurseControlsTester
{
	TEST_CLASS(TUITest)
	{
		TUI tui;

		TEST_METHOD_INITIALIZE(startTUI)
		{
			tui.init(); //stdscr is initialized after init is called
		}

		TEST_METHOD_CLEANUP(endTUI)
		{
			tui.shutdown();
		}

		TEST_METHOD(init_stdscrTest)
		{
			Assert::IsNotNull(stdscr);
		}

		TEST_METHOD(init_stdscrHeightTest)
		{
			Assert::AreEqual(LINES, getmaxy(stdscr));
		}

		TEST_METHOD(init_stdscrWidthTest)
		{
			Assert::AreEqual(COLS, getmaxx(stdscr));
		}

		TEST_METHOD(setBkgdColorTest)
		{
			chtype greenTile = setBkgdColor(COLOR_GREEN);
			Assert::IsTrue(greenTile == COLOR_GREEN << BKGDCOLOR_OFFSET);
		}

		TEST_METHOD(getBkgdColorTest)
		{
			chtype redTile = setBkgdColor(COLOR_RED);
			int color = getBkgdColor(redTile);

			Assert::IsTrue(color == COLOR_RED);
		}

		TEST_METHOD(setTextColorTest)
		{
			chtype blueTile = setTextColor(COLOR_BLUE);
			Assert::IsTrue(blueTile == COLOR_BLUE << TEXTCOLOR_OFFSET);
		}

		TEST_METHOD(getTextColorTest)
		{
			chtype yellowTile = setBkgdColor(COLOR_YELLOW);
			int color = getBkgdColor(yellowTile);

			Assert::IsTrue(color == COLOR_YELLOW);
		}


		TEST_METHOD(shutdownTest)
		{
			tui.shutdown();
			Assert::IsTrue(isendwin() != 0);
		}

		

	};
}