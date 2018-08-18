#include "CppUnitTest.h"
#include "BattleAlgorithm.h"
#include <iostream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace RPGLibTester
{
	TEST_CLASS(BattleAlgorithmTest)
	{

		/*
		Attack target with no defense
		*/
		TEST_METHOD(calcAttackDamageNoDefenseTest)
		{
			int attackerStrength = 13;
			int dmg = calcAttackDamage(attackerStrength, 0); //when target has no defense, damage matches attack strength
			Assert::AreEqual(dmg, attackerStrength);
		}
		
		/*
		Attack target with perfect defense
		*/
		TEST_METHOD(calcAttackDamagePerfectDefenseTest)
		{
			int attackerStrength = 156;
			int dmg = calcAttackDamage(attackerStrength, MAX_STAT_VALUE); //when target has perfect defense, damage is 0
			Assert::AreEqual(dmg, 0);
		}

		/*
		Attack target with half defense
		*/
		TEST_METHOD(calcAttackDamageHalfDefenseTest)
		{
			int attackerStrength = 79;
			int dmg = calcAttackDamage(attackerStrength, 127); //when target has half defense, damage is half attack strength
			Assert::AreEqual(dmg, attackerStrength / 2);
		}

		/*
		Attack enemy with no agility
		*/
		TEST_METHOD(enemyHasNoAgilityTest)
		{
			int dmg = 77;
			int agilityAdv = calcAgilityAdvantage(44, 91, 0); //enemy had some defense
			int finalDmg = dmg + agilityAdv;
			Assert::AreEqual(finalDmg, 92); //final damage is significantly higher than original damage
		}

		/*
		Attack enemy with perfect agility
		*/
		TEST_METHOD(enemyHasPerfectAgilityTest)
		{
			int dmg = 13; //making this match the attacker strength suggests enemy has no defense
			int agilityAdv = calcAgilityAdvantage(50, 13, MAX_STAT_VALUE);
			int finalDmg = dmg + agilityAdv;
			Assert::AreEqual(finalDmg, 2); //not much of the attack connects with a very agile target
		}

		/*
		Attack enemy with same agility as attacker
		*/
		TEST_METHOD(matchingAgilityTest) //values can be changed in test method without altering end result
		{
			int dmg = 46;
			int agility = 144; 
			int agilityAdv = calcAgilityAdvantage(agility, 60, agility); //enemy had some defense, attacker and target have same agility
			int finalDmg = dmg + agilityAdv;
			Assert::AreEqual(finalDmg, dmg); //final damage matches original damage when both have same agility
		}


		/*
		Use attack magic on enemy with no will
		*/
		TEST_METHOD(offensiveMagicNoWillTest)
		{
			double spellValue = 2.0;
			int intelligence = 15;
			int finalDmg = calcMagicDamage(spellValue, intelligence, 0); //enemy had some defense, attacker and target have same agility
			Assert::AreEqual(finalDmg, (int)(intelligence * spellValue)); //final damage matches intel * spellValue
		}

		/*
		Use attack magic on enemy with perfect will
		*/
		TEST_METHOD(offensiveMagicPerfectWillTest)
		{
			double spellValue = 5.0;
			int intelligence = 200;
			int finalDmg = calcMagicDamage(spellValue, intelligence, MAX_STAT_VALUE);
			Assert::AreEqual(finalDmg, 3); //final damage is greatly reduced
		}


		TEST_METHOD(applyVarianceTest)
		{
			int value = 100;
			int percentVariance = 20;

			int variedValue;
			bool inRange = true;
			
			int actualVariance = calcVariance(value, percentVariance);
			
			Assert::IsTrue(actualVariance <= percentVariance && actualVariance >= 1);
		}

		TEST_METHOD(applySmallVarianceTest)
		{
			int value = 1;
			int percentVariance = 20;

			int variedValue;
			bool inRange = true;

			int actualVariance = calcVariance(value, percentVariance);

			Assert::AreEqual(actualVariance, 0); //calcVariance returned 0 for small value
		}
	};
}