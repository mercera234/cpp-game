#include "CppUnitTest.h"
#include "CenteredFormat.h"
#include "TUI.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PDCurseControlsTester
{
	TEST_CLASS(CenteredFormatTest)
	{
		TUI tui;
		CenteredFormat fmt;

		TEST_METHOD(splitTextTest)
		{
			std::string text = "Here are some words for you.";

			fmt.splitText(13, text);
			
			Assert::AreEqual(3, (int)fmt.getLines().size());
			Assert::AreEqual(0, fmt.getLines().front().compare("Here are some"));
		}

		TEST_METHOD(drawTest)
		{
			int dim = 3;
			WINDOW* win = newwin(dim, dim, 0, 0);
			std::string text = "A";

			fmt.draw(win, text);

			int center = getPosition(Justf::CENTER, dim, 1); 
			chtype c = mvwinch(win, center, center); //get character in center of window

			Assert::AreEqual('A', (char)c);
			
			delwin(win);
		}

		TEST_METHOD(changeTextBetweenDrawTest)
		{
			int dim = 3;
			WINDOW* win = newwin(dim, dim, 0, 0);
			std::string text = "A";

			fmt.draw(win, text);
			fmt.draw(win, "B");

			int center = getPosition(Justf::CENTER, dim, 1);
			chtype c = mvwinch(win, center, center); //get character in center of window

			Assert::AreEqual('B', (char)c);

			delwin(win);
		}

	};
}