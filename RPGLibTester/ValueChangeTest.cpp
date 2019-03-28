#include "CppUnitTest.h"
#include "ValueChange.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace RPGLibTester
{
	TEST_CLASS(ValueChangeTest)
	{
		TUI tui;
		ValueChange val;

		TEST_METHOD_INITIALIZE(start)
		{
			val.setFmt(new PosFormat(0, 0));
		}

		TEST_METHOD(drawPositiveValueTest)
		{
			int v = 356;
			val.setValue(&v);
			val.draw(stdscr);

			char valStr[10];
			mvwinnstr(stdscr, 0, 0, valStr, 4);

			Assert::AreEqual("+356", valStr);
		}

		TEST_METHOD(drawNegativeValueTest)
		{
			int v = -8008;
			val.setValue(&v);
			val.draw(stdscr);

			char valStr[10];
			mvwinnstr(stdscr, 0, 0, valStr, 5);

			Assert::AreEqual("-8008", valStr);
		}

		TEST_METHOD(drawZeroTest)
		{
			int v = 0;
			val.setValue(&v);
			val.draw(stdscr);

			char valStr[10];
			mvwinnstr(stdscr, 0, 0, valStr, 1);

			Assert::AreEqual("0", valStr);
		}

		TEST_METHOD(drawNoZeroTest)
		{
			int v = 0;
			val.setValue(&v);
			val.setDrawZero(false);
			val.draw(stdscr);

			char valStr[10];
			mvwinnstr(stdscr, 0, 0, valStr, 1);

			Assert::AreEqual(" ", valStr);
		}

		TEST_METHOD(colorCheckTest)
		{
			int v = 1;
			val.setValue(&v);
			val.draw(stdscr);			

			chtype c = mvwinch(stdscr, 0, 0);
			
			Assert::AreEqual(COLOR_GREEN_BOLD, getTextColor(c));
		}
	};
}