#include "CppUnitTest.h"
#include "Direction.h"
#include "MockControl.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PDCurseControlsTester
{
	TEST_CLASS(DirectionTest)
	{
		TUI tui;

		TEST_METHOD(getOppositeDirTest)
		{
			Assert::AreEqual((int)Dir::UP, (int)getOppositeDir(Dir::DOWN));
			Assert::AreEqual((int)Dir::LEFT, (int)getOppositeDir(Dir::RIGHT));
		}

		TEST_METHOD(getAxisTest)
		{
			Assert::AreEqual((int)Axis::VERTICAL, (int)getAxis(Dir::DOWN));
			Assert::AreEqual((int)Axis::HORIZONTAL, (int)getAxis(Dir::RIGHT));
		}

		TEST_METHOD(getPerpAxisTest)
		{
			Assert::AreEqual((int)Axis::VERTICAL, (int)getPerpAxis(Axis::HORIZONTAL));
			Assert::AreEqual((int)Axis::HORIZONTAL, (int)getPerpAxis(Axis::VERTICAL));
		}

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


	/*	TEST_METHOD(addMovesTest)
		{
			Movement m1(Dir::RIGHT, 7);
			Movement m2(Dir::UP, 3);

			Vector v;
			addMoves(v, m1, m2);

			Assert::AreEqual(7, v.horzMove.magnitude);
			Assert::AreEqual((int)Dir::RIGHT, (int)v.horzMove.dir);
			Assert::AreEqual(3, v.vertMove.magnitude);
			Assert::AreEqual((int)Dir::UP, (int)v.vertMove.dir);
		}

		TEST_METHOD(addMovesSameAxisTest)
		{
			Movement m1(Dir::RIGHT, 7);
			Movement m2(Dir::LEFT, -3);

			Vector v;
			addMoves(v, m1, m2);

			Assert::AreEqual(10, v.horzMove.magnitude);
			Assert::AreEqual((int)Dir::RIGHT, (int)v.horzMove.dir);
			Assert::AreEqual(0, v.vertMove.magnitude);
		}*/
	};
}