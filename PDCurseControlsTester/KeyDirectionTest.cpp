#include "CppUnitTest.h"
#include "KeyDirection.h"
#include "MockControl.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PDCurseControlsTester
{
	TEST_CLASS(KeyDirectionTest)
	{
		TUI tui;

		TEST_METHOD(getDirectionFromKeyTest)
		{
			Assert::AreEqual((int)Dir::DOWN, (int)getDirectionFromKey(KEY_DOWN));
			Assert::AreEqual((int)Dir::UP, (int)getDirectionFromKey(KEY_UP));
			Assert::AreEqual((int)Dir::RIGHT, (int)getDirectionFromKey(KEY_RIGHT));
			Assert::AreEqual((int)Dir::LEFT, (int)getDirectionFromKey(KEY_LEFT));

			Assert::AreEqual((int)Dir::DOWN, (int)getDirectionFromKey(KEY_PGDN));
			Assert::AreEqual((int)Dir::UP, (int)getDirectionFromKey(KEY_PGUP));
			Assert::AreEqual((int)Dir::RIGHT, (int)getDirectionFromKey(CTL_PGDN));
			Assert::AreEqual((int)Dir::LEFT, (int)getDirectionFromKey(CTL_PGUP));

			Assert::AreEqual((int)Dir::DOWN, (int)getDirectionFromKey(KEY_SEND));
			Assert::AreEqual((int)Dir::UP, (int)getDirectionFromKey(KEY_SHOME));
			Assert::AreEqual((int)Dir::RIGHT, (int)getDirectionFromKey(KEY_END));
			Assert::AreEqual((int)Dir::LEFT, (int)getDirectionFromKey(KEY_HOME));
		}

		TEST_METHOD(getMagnitudeFromKeyTest)
		{
			MockControl control;
			WINDOW* win = dupwin(stdscr);
			control.setWindow(win);

			Assert::AreEqual(1, getMagnitudeFromKey(KEY_DOWN));
			Assert::AreEqual(-1, getMagnitudeFromKey(KEY_UP));
			Assert::AreEqual(1, getMagnitudeFromKey(KEY_RIGHT));
			Assert::AreEqual(-1, getMagnitudeFromKey(KEY_LEFT));

			Assert::AreEqual(getmaxy(win), getMagnitudeFromKey(KEY_PGDN, &control));
			Assert::AreEqual(-getmaxy(win), getMagnitudeFromKey(KEY_PGUP, &control));
			Assert::AreEqual(getmaxx(win), getMagnitudeFromKey(CTL_PGDN, &control));
			Assert::AreEqual(-getmaxx(win), getMagnitudeFromKey(CTL_PGUP, &control));

			Assert::AreEqual((int)control.getTotalRows(), getMagnitudeFromKey(KEY_SEND, &control));
			Assert::AreEqual(-(int)control.getTotalRows(), getMagnitudeFromKey(KEY_SHOME, &control));
			Assert::AreEqual((int)control.getTotalCols(), getMagnitudeFromKey(KEY_END, &control));
			Assert::AreEqual(-(int)control.getTotalCols(), getMagnitudeFromKey(KEY_HOME, &control));
			delwin(win);
		}



	};
}