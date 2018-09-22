#include "CppUnitTest.h"
#include "ResourceManager.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace RPGLibTester
{
	TEST_CLASS(ResourceManagerTest)
	{
		TUI tui;
		ResourceManager rm;

		TEST_METHOD(getNullResourcesTest)
		{
			rm.loadNullResources();

			Assert::AreEqual(1, (int)rm.actors.size()); 
			Assert::AreEqual(1, (int)rm.mapRooms.size()); 
			Assert::AreEqual(1, (int)rm.gameMaps.size());
		}

		TEST_METHOD(getNullResourcesActorContentTest)
		{
			rm.loadNullResources();
			Actor a = rm.actors.find(nullName)->second;

			Assert::AreEqual((char)nullSymbol, (char)a.symbol); 
		}

		TEST_METHOD(getNullResourcesMapContentTest)
		{
			rm.loadNullResources();
			MapRoom m = rm.mapRooms.find(nullId)->second;
			TwoDStorage<chtype>* tileMap = m.getDisplay()->getTileMap();
			chtype c = tileMap->getDatum(4, 8);

			Assert::AreEqual('!', (char)c); 
		}

		TEST_METHOD(getNextIdTest)
		{
			rm.resetNextId();
			Assert::AreEqual(0, rm.getNextId()); //without being called before, next id will be 0
		}
	};
}