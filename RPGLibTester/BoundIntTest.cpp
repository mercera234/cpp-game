#include "CppUnitTest.h"
#include "BoundInt.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace RPGLibTester
{
	TEST_CLASS(BoundIntTest)
	{
		TEST_METHOD(defaultCnstr)
		{
			BoundInt level;

			Assert::AreEqual(INT_MAX, level.getMax()); //will equal max since it exceeds the max
		}

		TEST_METHOD(setValues)
		{
			BoundInt level;
			level.setValues(3, 78);

			Assert::AreEqual(3, level.getCurr()); //will equal min since it is originally lower than min
		}

		TEST_METHOD(BoundIntCnstr)
		{
			int min = 0;
			int max = 99;
			BoundInt level(min, max);
			
			Assert::AreEqual(min, level.getMin());
			Assert::AreEqual(max, level.getCurrMax());
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

		TEST_METHOD(setCurrMaxLow) //set max below current, and current is auto modified
		{
			int min = 0;
			int max = 999;
			BoundInt mp(min, max);

			mp.setCurr(500);
			int newMax = 499;
			mp.setCurrMax(newMax);
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

		TEST_METHOD(setTempMax)
		{
			int min = 0;
			int max = 9999;
			BoundInt heroHp(min, max);

			heroHp.setCurrMax(50);

			int tempMax = 100;
			heroHp.setTempMax(tempMax);
			heroHp.maxOut();

			Assert::AreEqual(tempMax, heroHp.getCurr());
		}

		TEST_METHOD(resetTempMax)
		{
			int min = 0;
			int max = 9999;
			BoundInt heroHp(min, max);

			int currMax = 50;
			heroHp.setCurrMax(currMax);

			int tempMax = 100;
			heroHp.setTempMax(tempMax);
			heroHp.resetTempMax();

			Assert::AreEqual(currMax, heroHp.getTempMax());
		}

		TEST_METHOD(alterCurr)
		{
			BoundInt heroHp(0, 300, 100);
			heroHp.alterCurr(-30);
			heroHp.alterCurr(2);

			Assert::AreEqual(72, heroHp.getCurr());
		}

	};
}