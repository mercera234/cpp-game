#include "CppUnitTest.h"
#include "Divider.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PDCurseControlsTester
{
	TEST_CLASS(DividerTest)
	{
		TUI tui;
		TEST_METHOD(dividerDrawFullLineTest)
		{
			Divider d(Pos(0, 0), Axis::HORIZONTAL); //length not specified for test

			d.draw(stdscr);

			chtype start = mvwinch(stdscr, 0, 0);
			chtype end = mvwinch(stdscr, 0, getmaxx(stdscr) - 1);

			Assert::AreEqual(ACS_HLINE, start);
			Assert::AreEqual(ACS_HLINE, end);
		}

		TEST_METHOD(dividerDrawTest)
		{
			Divider d(Pos(0, 0), Axis::HORIZONTAL, getmaxx(stdscr) - 1);

			d.draw(stdscr);

			Assert::AreEqual((int)' ', (int)mvwinch(stdscr, 0, getmaxx(stdscr) - 1));
			Assert::AreEqual((int)ACS_HLINE, (int)mvwinch(stdscr, 0, getmaxx(stdscr) - 2));
		}
	};
}