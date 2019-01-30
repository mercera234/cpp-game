#include "CppUnitTest.h"
#include "MainMenu.h"
#include "defaults.h"
#include "GameInput.h"

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

		TEST_METHOD(setResourceManagerTest)
		{
			mm.setResourceManager(&rm);

			Assert::IsTrue(mm.getResourceManager() == &rm);
		}

		/*TEST_METHOD(cursorDefaultPositionTest)
		{
			mm.setResourceManager(&rm);
			

			Assert::IsTrue(mm.getResourceManager() == &rm);
		}*/
	};
}