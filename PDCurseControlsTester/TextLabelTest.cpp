#include "stdafx.h"
#include "CppUnitTest.h"
#include "TextLabel.h"
#include "TUI.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PDCurseControlsTester
{
	TEST_CLASS(TextLabelTest)
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

		TEST_METHOD(getTextTest)
		{
			std::string txt = "Test";
			TextLabel label(stdscr, txt);
			Assert::AreEqual(label.getText().compare(txt), 0);
		}

		TEST_METHOD(setTextTest)
		{
			std::string txt = "Test";
			TextLabel label(stdscr, txt);

			std::string newTxt = "Here's a long string";
			label.setText(newTxt);
			Assert::AreEqual(label.getText().compare(newTxt), 0);
		}

		
	};
}