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