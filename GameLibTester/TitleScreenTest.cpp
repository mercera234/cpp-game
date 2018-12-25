#include "CppUnitTest.h"
#include "TitleScreen.h"
#include "defaults.h"
#include "Style.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace GameLibTester
{
	TEST_CLASS(TitleScreenTest)
	{
		TUI tui;

		TEST_METHOD(ctorTest)
		{
			TitleScreen ts;

			WINDOW* win = ts.getWindow();

			Assert::AreEqual(gameScreenHeight, getmaxy(win));
			Assert::AreEqual(gameScreenWidth, getmaxx(win));
		}

		TEST_METHOD(drawTest)
		{
			TitleScreen ts;

			ts.draw();
			
			WINDOW* win = ts.getWindow();

			int row = getPosition(Justf::CENTER, 10, 1); //top part of window has height of 10
			int col = getPosition(Justf::CENTER, getmaxx(win), GAME_TITLE.length());
			
			char buf[100];
			mvwinnstr(win, row, col, buf, GAME_TITLE.length());
			Assert::AreEqual(buf, GAME_TITLE.c_str());
		}
	};
}