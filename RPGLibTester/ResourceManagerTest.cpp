#include "CppUnitTest.h"
#include "ResourceManager.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace RPGLibTester
{
	TEST_CLASS(ResourceManagerTest)
	{
		ResourceManager rm;

		TEST_METHOD(getActorsTest)
		{
			std::ifstream is;
			int actorsLoaded = rm.loadActorsFromTextFile(is);

			Assert::AreEqual(0, actorsLoaded); //loads nothing since we aren't connecting to filesystem
		}

		TEST_METHOD(getNullResourcesTest)
		{
			rm.loadNullResources();

			Assert::AreEqual(1, (int)rm.actors.size()); 
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
			Map m = rm.gameMaps.find(nullId)->second;
			TwoDStorage<chtype>* tileMap = m.getDisplay()->getTileMap();
			chtype c = tileMap->getDatum(4, 8);

			Assert::AreEqual('!', (char)c); 
		}
	};
}