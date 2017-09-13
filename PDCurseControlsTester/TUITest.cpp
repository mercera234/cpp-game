#include "stdafx.h"
#include "CppUnitTest.h"
#include "TUI.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PDCurseControlsTester
{
	TEST_CLASS(TUITest)
	{
		TEST_METHOD(init_stdscrTest)
		{
			TUI tui;
			tui.init();
			//stdscr is initialized after init is called
			Assert::IsNotNull(stdscr);
		}

		TEST_METHOD(init_stdscrHeightTest)
		{
			TUI tui;
			tui.init();

			//stdscr is initialized after init is called
			Assert::AreEqual(LINES, getmaxy(stdscr));
		}

		TEST_METHOD(init_stdscrWidthTest)
		{
			TUI tui;
			tui.init();

			//stdscr is initialized after init is called
			Assert::AreEqual(COLS, getmaxx(stdscr));
		}

		TEST_METHOD(shutdownTest)
		{
			TUI tui;
			tui.init();
			tui.shutdown();
			Assert::IsTrue(isendwin());
		}

	};
}