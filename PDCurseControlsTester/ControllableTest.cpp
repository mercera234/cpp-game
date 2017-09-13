#include "stdafx.h"
#include "CppUnitTest.h"
#include "TUI.h"
#include "MockControl.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PDCurseControlsTester
{
	TEST_CLASS(ControllableTest)
	{
		TUI tui;
		MockControl control;

		TEST_METHOD_INITIALIZE(startTUI)
		{
			tui.init();
		}

		TEST_METHOD_CLEANUP(stopTUI)
		{
			tui.shutdown();
		}

		TEST_METHOD(getWindowTest)
		{
			control.setWindow(stdscr);
			bool sameWindow = stdscr == control.getWindow();
			Assert::IsTrue(sameWindow);
		}

		TEST_METHOD(shiftTest)
		{
			int y = 2;
			int x = 7;
			control.shift(2, 7);

			Assert::AreEqual(y, control.getUlY());
		}

		TEST_METHOD(setDimensionsTest)
		{
			int rows = 6;
			int cols = 17;
			control.setDimensions(rows, cols);
			
			Assert::AreEqual(rows, (int)control.getTotalRows());
			Assert::AreEqual(cols, (int)control.getTotalCols());
		}
	};
}