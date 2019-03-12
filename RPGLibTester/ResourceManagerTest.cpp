#include "CppUnitTest.h"
#include "ResourceManager.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace RPGLibTester
{
	TEST_CLASS(ResourceManagerTest)
	{
		ResourceManager rm;
		
		TEST_METHOD(getPlayer1NoActorTest)
		{
			Actor* a = rm.getPlayer1();

			Assert::AreEqual((int)nullptr, (int)a);
		}
		
		TEST_METHOD(getPlayer1Test)
		{
			Actor a;
			a.name = "Test";
			rm.playerParty.push_back(a);
			Actor* ptr = rm.getPlayer1();

			Assert::IsTrue(ptr->name.compare(a.name) == 0);
		}

		TEST_METHOD(acquireFreshItemTest)
		{
			GameItem potion;
			potion.name = "Potion";

			Sprite s;
			s.thing = &potion;
			s.quantity = 1;
			
			rm.acquireItem((GameItem*)s.thing, s.quantity);
			Assert::AreEqual(1, (int)rm.inventory.size()); //only item in inventory
		}

		TEST_METHOD(acquireItemAddTest)
		{
			GameItem potion;
			potion.name = "Potion";

			Sprite s;
			s.thing = &potion;
			s.quantity = 4;

			rm.acquireItem((GameItem*)s.thing, s.quantity);
			rm.acquireItem((GameItem*)s.thing, 3);
			Assert::AreEqual(1, (int)rm.inventory.size()); //only item in inventory
			Assert::AreEqual(7, rm.inventory.front()->quantity.getCurr()); //only quantity of item
		}
		
	};
}