#include "CppUnitTest.h"
#include <map>
#include <functional>
#include "KeyComparators.h"
#include "Actor.h"
#include "Map.h"
#include "GameItem.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace RPGLibTester
{
	TEST_CLASS(RepositoryTest)
	{
		TEST_METHOD(simpleMapTest)
		{
			std::map<int, int> repo;
			repo.insert(std::make_pair(1, 2));
		
			Assert::AreEqual(1, (int)repo.size());
		}

		TEST_METHOD(actorMapTest)
		{
			std::map<std::string, Actor, std::function<bool(std::string, std::string)> > actorRepo(stringCompare);
			std::string s;
			Actor a;
			actorRepo.insert(std::make_pair(s, a));

			Assert::AreEqual(1, (int)actorRepo.size());
		}

		TEST_METHOD(gameMapMapTest)
		{
			std::map<int, Map> mapRepo;
			
			Map m;
			mapRepo.insert(std::make_pair(0, m));

			Assert::AreEqual(1, (int)mapRepo.size());
		}

		TEST_METHOD(gameItemMapTest)
		{
			std::map<int, GameItem> itemRepo;

			GameItem item;
			itemRepo.insert(std::make_pair(0, item));

			Assert::AreEqual(1, (int)itemRepo.size());
		}
	};
}