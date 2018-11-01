#include "CppUnitTest.h"
#include "ExploreState.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace GameLibTester
{
	TEST_CLASS(ExploreStateTest)
	{
		GameState* exploreState;
		ResourceManager rm;
		TUI tui;

		TEST_METHOD_INITIALIZE(start)
		{
			exploreState = ExploreState::getInstance();
		}


		TEST_METHOD(getInstanceTest)
		{
			Assert::IsTrue(exploreState != nullptr);
		}

		TEST_METHOD(initDefaultsTest)
		{
			exploreState->setResourceManager(&rm);
			exploreState->initDefaults();
			Assert::IsTrue(true);
		}

	};
}