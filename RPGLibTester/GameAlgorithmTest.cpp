#include "CppUnitTest.h"
#include "GameAlgorithm.h"
#include "Actor.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace RPGLibTester
{
	TEST_CLASS(GameAlgorithmTest)
	{
		TEST_METHOD(toNextTest)
		{
			Actor player;
			Assert::AreEqual(12, alg::toNext(player));
		}

		TEST_METHOD(toNextLevel2Test)
		{
			Actor player;
			alg::gainExp(player, 12);//this should raise level to 2

			Assert::AreEqual(35, alg::toNext(player));
		}

		TEST_METHOD(getLevelDataTest)
		{
			Assert::AreEqual(6637181, (int)alg::getLevelData(99, StatType::EXP));
		}

		TEST_METHOD(gainExpTest)
		{
			Actor player;

			int amount = 46; //47 would trigger 2 levels gained
			int levelsGained = alg::gainExp(player, amount);

			Assert::AreEqual(1, levelsGained);
		}
	};
}