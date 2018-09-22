#include "CppUnitTest.h"
#include "BattleProcessor.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace GameLibTester
{
	TEST_CLASS(BattleProcessorTest)
	{
		TUI tui;

		TEST_METHOD(noParticipantTest)
		{
			BattleProcessor bp;

			//create empty lists
			std::list<Actor*> players;
			std::list<Actor*> enemies;
			bp.setWindow(stdscr); 
			bp.addParticipants(players, enemies);

			Assert::IsTrue(true); //not a good test
		}
	};
}