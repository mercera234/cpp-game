#include <functional>
#include <iostream>
#include "CppUnitTest.h"
#include "TurnTracker.h"
#include "MockPlayer.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PDCurseControlsTester
{
	bool predTest()
	{
		return true;
	};

	TEST_CLASS(TurnTrackerTest)
	{
		TurnTracker tt;

		MockPlayer player1;
		MockPlayer player2;

		TEST_METHOD_INITIALIZE(setup)
		{
			player1.id = 1;
			player2.id = 2;
		}


		TEST_METHOD(constructorTest)
		{
			Assert::IsTrue(tt.getActive());
		}
		
		/*
		A single player should be capable of being added to the TurnTracker.
		*/
		TEST_METHOD(addPlayerToTrackerTest)
		{
			tt.addPlayer(&player1);
			Assert::AreEqual(1, tt.getPlayerCount());
		}


		/*
		It should be possible to add a group of players at one time through a single method call
		*/
		TEST_METHOD(addPlayersToTrackerTest)
		{
			std::list<Player*> players;
			players.push_back(&player1);
			players.push_back(&player2);

			tt.addPlayers(players);
			Assert::AreEqual(2, tt.getPlayerCount());
		}

		/*
		All the players can be removed from the TurnTracker at one time
		*/
		TEST_METHOD(freeTest)
		{
			std::list<Player*> players;
			players.push_back(&player1);
			players.push_back(&player2);

			tt.addPlayers(players);

			tt.free();

			Assert::AreEqual(0, tt.getPlayerCount());
		}


		/*
		It is possible to request for the next player
		*/
		TEST_METHOD(getNextTest)
		{
			tt.addPlayer(&player1);

			Player* p = tt.getNext();

			Assert::IsTrue(p == &player1);
		}

		/*
		Will not be able to retrieve a player that can't play
		*/
		TEST_METHOD(getNextOneDeadTest)
		{
			player2.mockOn = false; //can't play
			tt.addPlayer(&player1);
			tt.addPlayer(&player2);

			MockPlayer* p = (MockPlayer*)tt.getNext();
			MockPlayer* p2 = (MockPlayer*)tt.getNext();

			//we get next twice and both times it is the only player that can play
			Assert::AreEqual(1, p->id);
			Assert::AreEqual(1, p2->id);
		}

		/*
		Will retrieve no player if no one can play
		*/
		TEST_METHOD(getNextFailTest)
		{
			player1.mockOn = false; //can't play
			tt.addPlayer(&player1);

			Player* p = tt.getNext();

			//we get next twice and both times it is the only player that can play
			Assert::IsTrue(nullptr == p);	
		}

		/*
		Inactive player should be added to round if not able to play
		*/
		TEST_METHOD(getNextRevivedTest)
		{
			player1.mockOn = false; //can't play
			
			tt.addPlayer(&player1);
			tt.addPlayer(&player2);

			Player* p = tt.getNext();//guaranteed to be player2 since player1 can't play

			player1.mockOn = true;

			MockPlayer* p2 = (MockPlayer*)tt.getNext();//guaranteed to be player1 since player2 was already removed from queue

			Assert::AreEqual(1, p2->id);
		}

		/*
		A queued up player should be skipped if can't play before turn comes up
		*/
		TEST_METHOD(getNextSkippedTest)
		{
			tt.addPlayer(&player1);
			tt.addPlayer(&player2);

			player1.mockOn = false; //player1 can't play now

			Player* p = tt.getNext();//guaranteed to be player2 since player1 can't play

			MockPlayer* p2 = (MockPlayer*)tt.getNext();//guaranteed to be player2 since player1 still can't play

			Assert::AreEqual(2, p2->id);
		}


		

	};
}