#include "CppUnitTest.h"
#include "BattleProcessor.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace RPGLibTester
{
	TEST_CLASS(BattleProcessorTest)
	{
		TUI tui;

		TEST_METHOD_INITIALIZE(startTUI)
		{
			tui.init(); //stdscr is initialized after init is called
		}

		TEST_METHOD_CLEANUP(endTUI)
		{
			tui.shutdown();
		}

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