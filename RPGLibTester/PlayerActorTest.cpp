#include "CppUnitTest.h"
#include "PlayerActor.h"
#include "TurnTracker.h"
#include "actor_helper.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace RPGLibTester
{
	TEST_CLASS(PlayerActorTest)
	{
		TurnTracker tt;

		PlayerActor p1;
		PlayerActor p2;

		Actor a1;
		Actor a2;

		int deadHP = 0;
		int aliveHP = 1;

		TEST_METHOD_INITIALIZE(setup)
		{
			a1.symbol = '1';
			a2.symbol = '2';

			p1.setActor(&a1);
			p2.setActor(&a2);
		}

		TEST_METHOD(canAliveActorPlayTest)
		{
			setBoundedStat(a1.stats.hp, 0, 25, 9999, aliveHP);
			Assert::IsTrue(p1.canPlay());
		}

		TEST_METHOD(canDeadActorPlayTest)
		{
			setBoundedStat(a1.stats.hp, 0, 25, 9999, deadHP);
			Assert::IsFalse(p1.canPlay());
		}

		/*
		A single player should be capable of being added to the TurnTracker.
		*/
		TEST_METHOD(addPlayerToTrackerTest)
		{
			tt.addPlayer(&p1);
			Assert::AreEqual(1, tt.getPlayerCount());
		}

		/*
		Will not be able to retrieve a player that can't play
		*/
		TEST_METHOD(getNextOneDeadTest)
		{
			setBoundedStat(a1.stats.hp, 0, 25, 9999, aliveHP);
			setBoundedStat(a2.stats.hp, 0, 25, 9999, deadHP);

			tt.addPlayer(&p1);
			tt.addPlayer(&p2);

			Player* pl1 = tt.getNext();
			Player* pl2 = tt.getNext();

			//both players were added, but the same player is returned both times because one is dead
			Assert::AreEqual((int)pl1, (int)pl2);
		}

		/*
		A queued up player should be skipped if can't play before turn comes up
		*/
		TEST_METHOD(getNextSkippedTest)
		{
			setBoundedStat(a1.stats.hp, 0, 25, 9999, aliveHP);
			setBoundedStat(a2.stats.hp, 0, 25, 9999, aliveHP);
	
			tt.addPlayer(&p1);
			tt.addPlayer(&p2);

			alterStatValue(a1.stats.hp, -10); //killed, player1 can't play now
			
			PlayerActor* p = (PlayerActor*)tt.getNext();//guaranteed to be player2 since player1 can't play
			PlayerActor* p2 = (PlayerActor*)tt.getNext();//guaranteed to be player2 since player1 can't play still

			Assert::AreEqual((char)(p->getActor()->symbol), '2');
			Assert::AreEqual((int)p, (int)p2);
		}




	};
}