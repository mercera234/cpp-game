#include "CppUnitTest.h"
#include "BoundInt.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace RPGLibTester
{
	TEST_CLASS(BoundIntTest)
	{
		TEST_METHOD(BoundIntCnstr)
		{
			int min = 0;
			int max = 99;
			BoundInt level(min, max);
			
			Assert::AreEqual(min, level.getMin());
			Assert::AreEqual(max, level.getMax());
		}

		TEST_METHOD(BoundIntCnstrWithCurr)
		{
			int min = 0;
			int max = 99;
			BoundInt level(min, max, 200);

			Assert::AreEqual(max, level.getCurr()); //will equal max since it exceeds the max
		}

		TEST_METHOD(setCurrLow)
		{
			int min = 0;
			int max = 99;
			BoundInt level(min, max);

			level.setCurr(-1);
			Assert::AreEqual(min, level.getCurr());
		}

		TEST_METHOD(setCurrHigh)
		{
			int min = 0;
			int max = 9999;
			BoundInt hp(min, max);

			hp.setCurr(10000);
			Assert::AreEqual(max, hp.getCurr());
		}

		TEST_METHOD(setMaxLow) //set max below current, and current is auto modified
		{
			int min = 0;
			int max = 999;
			BoundInt mp(min, max);

			mp.setCurr(500);
			int newMax = 499;
			mp.setMax(newMax);
			Assert::AreEqual(newMax, mp.getCurr());
		}

		TEST_METHOD(setMinHigh)
		{
			int min = -1000;
			int max = 1000;
			BoundInt money(min, max);

			money.setCurr(-456);
			int newMin = 0;
			money.setMin(newMin);
			Assert::AreEqual(newMin, money.getCurr());
		}

		TEST_METHOD(maxOut)
		{
			int min = 0;
			int max = 999999;
			BoundInt enemyHp(min, max);

			enemyHp.setCurr(1);
			enemyHp.maxOut();

			Assert::AreEqual(max, enemyHp.getCurr());
		}

	};
}