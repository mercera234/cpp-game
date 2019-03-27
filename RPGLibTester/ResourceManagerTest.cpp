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

		
		
	};
}