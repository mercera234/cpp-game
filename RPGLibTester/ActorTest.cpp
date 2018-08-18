#include "CppUnitTest.h"
#include "Actor.h"
#include "actor_helper.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace RPGLibTester
{
	TEST_CLASS(ActorTest)
	{
		TEST_METHOD(gainMoney)
		{
			Actor* player = buildActor(ActorType::HUMAN);
			player->money.setCurr(0); //should be 0 by default, but this is more explicit

			int moneyGain = 100;
			alterStatValue(player->money, moneyGain);
			
			Assert::AreEqual(moneyGain, player->money.getCurr());
		}

		TEST_METHOD(maxOutHP) //like if using a restorative item or sleeping at an inn
		{
			Actor player;
			int maxHP = 50;

			BoundInt* hp = &(player.stats.hp);

			hp->setMax(maxHP);
			hp->setCurr(10);

			hp->maxOut();

			Assert::AreEqual(maxHP, hp->getCurr());
		}

		TEST_METHOD(gainExperience) //no level up
		{
			Actor* player = buildActor(ActorType::HUMAN);

			player->stats.exp.setCurr(100);
			
			player->alterStat(StatType::EXP, 10);
			
			Assert::AreEqual(110, player->stats.exp.getCurr());
		}

		TEST_METHOD(damageActor)
		{
			Actor* player = buildActor(ActorType::HUMAN);

			player->stats.hp.setCurr(25);

			player->alterStat(StatType::HP, -10);

			Assert::AreEqual(15, player->stats.hp.getCurr());
		}

		TEST_METHOD(healActor)
		{
			Actor* player = buildActor(ActorType::HUMAN);

			int maxHP = 25;
			player->stats.hp.setMax(maxHP);
			player->stats.hp.setCurr(1);

			player->alterStat(StatType::HP, 30); //player is healed beyond max

			Assert::AreEqual(maxHP, player->stats.hp.getCurr()); 
		}

		TEST_METHOD(isActorAlive)
		{
			Actor* player = buildActor(ActorType::HUMAN);

			Assert::IsTrue(isAlive(*player));
		}

		TEST_METHOD(isActorDead)
		{
			Actor* player = buildActor(ActorType::HUMAN);

			player->alterStat(StatType::HP, -10000);

			Assert::IsFalse(isAlive(*player));
		}
		



		//move actor
		//actor gets status affliction
		//actor cures status affliction
		//actor gets item
		//actor uses item
		//gain experience with level gain


		TEST_METHOD(getItem)
		{
			/*
			Actor player;
			Item item("Potion");
			player.getItem(item);
			
			Assert::IsTrue(player.hasItem(item));
			*/

		}


		TEST_METHOD(useItem)
		{
			/*
			Actor player;
			player.maxHp = 20;
			player.currHp = player.maxHp;
			Item item("Potion");
			player.getItem(item);
			player.takeDamage(10);


			Assert::IsTrue(player.hasItem(item));
			*/

		}
	};
}