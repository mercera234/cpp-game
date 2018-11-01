#include "CppUnitTest.h"
#include "LineFormat.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PDCurseControlsTester
{
	TEST_CLASS(FormatTest)
	{
		TUI tui;

		TEST_METHOD(lineFormatDrawTestEasy)
		{
			LineFormat format;

			format.draw(stdscr, "7g");

			chtype c7 = mvwinch(stdscr, 0, 0);
			chtype cg = mvwinch(stdscr, 0, 1);

			char s[3] = { (char)c7, (char)cg, 0 };
			std::string line(s);

			Assert::AreEqual(0, line.compare("7g"));
		}
	};
}