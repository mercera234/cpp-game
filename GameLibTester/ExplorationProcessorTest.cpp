#include "CppUnitTest.h"
#include "ExplorationProcessor.h"
#include "actor_helper.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace GameLibTester
{
	TEST_CLASS(ExplorationProcessorTest)
	{
		TUI tui;
		ExplorationProcessor ep;

		TEST_METHOD(setResourceManagerTest)
		{
			ResourceManager rm;

			ep.setResourceManager(&rm);

			Assert::IsTrue(ep.getResourceManager() == &rm);
		}

		TEST_METHOD(setControlActorTest)
		{
			Actor player;
			initTestActor(player);

			ep.setControlActor(&player);

			Assert::IsTrue(ep.getControlActor() == &player);
		}

		TEST_METHOD(setCursorTest)
		{
			int x = 1;
			int y = 1;
			Pos pos(y, x);
			ResourceManager rm;
			ep.setResourceManager(&rm);

			MegaMap map;
			map.setDimensions(1, 1);
			map.setUnitHeight(23);
			map.setUnitWidth(51);
			Image img;
			img.setDimensions(1, 1);
			map.setLayerImage(0, img);

			ep.setMap(&map);
			ep.setCursor(pos);

			Assert::IsTrue(ep.getCursor().y == y);
		}

	};
}