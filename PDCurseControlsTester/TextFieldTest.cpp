#include "CppUnitTest.h"
#include "TextField.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PDCurseControlsTester
{
	TEST_CLASS(TextFieldTest)
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

		TEST_METHOD(constructorTest)
		{
			TextField field(16, 0, 0);
			std::string text = field.getText();
			Assert::IsTrue(0 == text.compare(""));
		}

		TEST_METHOD(clearTest)
		{
			TextField field(16, 0, 0);

			std::string text = "Cleared";
			field.setText(text);
			field.clear();

			std::string text2 = field.getText();
			Assert::IsTrue(0 == text2.compare(""));
		}

		TEST_METHOD(inputCharOneCharTest)
		{
			TextField field(30, 1, 1);

			field.inputChar('t');

			std::string text = field.getText();
			Assert::IsTrue(0 == text.compare("t"));
		}

		TEST_METHOD(inputCharInsertBeforeOtherTextTest)
		{
			TextField field(13, 5, 6);

			field.inputChar('2');
			field.inputChar(KEY_LEFT);
			field.inputChar('1');

			std::string text = field.getText();
			Assert::IsTrue(0 == text.compare("12"));
		}

		TEST_METHOD(inputCharDeleteCharTest)
		{
			TextField field(30, 1, 1);

			field.inputChar('8');
			field.inputChar(KEY_LEFT);
			field.inputChar(KEY_DC);

			std::string text = field.getText();
			Assert::IsTrue(0 == text.compare(""));
		}

		TEST_METHOD(inputCharBackspaceTest)
		{
			TextField field(30, 1, 1);

			field.inputChar('^');
			field.inputChar('$');
			field.inputChar('\b');

			std::string text = field.getText();
			Assert::IsTrue(0 == text.compare("^"));
		}


		TEST_METHOD(inputCharHomeTest)
		{
			TextField field(30, 1, 1);
			field.setText("Some words");
			field.inputChar(KEY_HOME);
			field.inputChar(KEY_DC);
			
			std::string text = field.getText();
			Assert::IsTrue(0 == text.compare("ome words"));
		}

		TEST_METHOD(inputCharEndTest)
		{
			TextField field(30, 1, 1);
			field.setText("Some words");
			field.inputChar(KEY_HOME);
			field.inputChar(KEY_DC);
			field.inputChar('B');
			field.inputChar(KEY_END);
			field.inputChar('.');

			std::string text = field.getText();
			Assert::IsTrue(0 == text.compare("Bome words."));
		}

		TEST_METHOD(cursorFocusTest)
		{
			int row = 2;
			int col = 3;

			TextField field(15, row, col);
			field.setFocus(true);

			int x, y;
			getyx(stdscr, y, x);

			Assert::AreEqual(row, y);
			Assert::AreEqual(col, x);
		}
	};
}