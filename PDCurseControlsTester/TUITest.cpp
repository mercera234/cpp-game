#include "CppUnitTest.h"
#include "TUI.h"
#include <Windows.h>

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

		TEST_METHOD(getMouseTest) //get mouse event without mouse click 
		{
			simulateMouseOn(false);
			getMouse();
			MEVENT* m = &TUI::mouseEvent;

			Assert::AreEqual(0, m->x);
			Assert::AreEqual(0, m->y);
			Assert::AreEqual(0, (int)m->bstate);
		}

		TEST_METHOD(simulateMouseTest) //simulate mouse click 
		{
			int y = 4;
			int x = 6;
			simulateMouseOn(true);
			setMouseEvent(y, x);
			getMouse();
			MEVENT* m = &TUI::mouseEvent;

			Assert::AreEqual(x, m->x);
			Assert::AreEqual(y, m->y);
		}

		TEST_METHOD(shutdownTest)
		{
			tui.shutdown();
			Assert::IsTrue(isendwin() != 0);
		}

		//Console windows are not instantiated by unit test
		/*TEST_METHOD(consoleCenterTest)
		{
			TUI::centerConsoleWindow(100, 200);

			HWND consoleWindow = GetConsoleWindow();
			RECT consoleRect;
			GetWindowRect(consoleWindow, &consoleRect);

			Assert::AreEqual(100, (int)(consoleRect.bottom - consoleRect.top));
		}*/
		

	};
}