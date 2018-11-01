#include "CppUnitTest.h"
#include "TextLabel.h"
#include "TUI.h"
#include "CenteredFormat.h"
#include <string>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PDCurseControlsTester
{
	TEST_CLASS(TextLabelTest)
	{
		TUI tui;

		TEST_METHOD(getTextTest)
		{
			std::string txt = "Test";
			TextLabel label(stdscr, txt);
			Assert::AreEqual(txt.c_str(), label.getText().c_str());
		}

		TEST_METHOD(setTextTest)
		{
			std::string txt = "Test";
			TextLabel label(stdscr, txt);

			std::string newTxt = "Here's a long string";
			label.setText(newTxt);
			Assert::AreEqual(newTxt.c_str(), label.getText().c_str());
		}


		TEST_METHOD(useCenterFormatTest)
		{
			WINDOW* win = newwin(3, 6, 0, 0);
			std::string txt = "Test";
			TextLabel label(win, txt);

			label.setFormat(new CenteredFormat);
			label.draw();

			char str[10];
			mvwinnstr(win, 1, 1, str, 4);

			Assert::AreEqual(txt.c_str(), str);
			delwin(win);
		}
	};
}