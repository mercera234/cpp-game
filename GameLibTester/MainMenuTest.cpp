#include "CppUnitTest.h"
#include "MainMenu.h"
#include "defaults.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace GameLibTester
{
	TEST_CLASS(MainMenuTest)
	{
		MainMenu mm;
		ResourceManager rm;

		TEST_METHOD_INITIALIZE(start)
		{
			//loadDataFiles(rm); TODO can game data be loaded in memory for testing?
		}


		TEST_METHOD(addPlayerPartyTest)
		{
			std::vector<Actor> players;
			mm.addPlayerParty(players);

			Assert::IsTrue(true); //TODO no means to check if successful yet
		}

		TEST_METHOD(setResourceManagerTest)
		{
			//added the bare minimum to get this to work
			MegaMap map;
			Pos pos(1, 1);
			map.setCursor(&pos.y, &pos.x);
			map.setDimensions(2, 2);

			Image img;
			img.setDimensions(2, 2);
			img.reset();
			map.setLayerImage(0, img);

			rm.currMap = &map;

			setupDefaultDataKeys(rm);

			mm.setResourceManager(&rm);

			Assert::IsTrue(true);
		}
	};
}