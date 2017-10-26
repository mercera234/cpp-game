#include "CppUnitTest.h"
#include "GameStateManager.h"
#include "MockState1.h"
#include "MockState2.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PDCurseControlsTester
{
	TEST_CLASS(StateManagerTest)
	{
		GameStateManager manager; //no state added

		TEST_METHOD(processInputOnEmpty)
		{
			Assert::IsFalse(manager.processInput(78)); //any input will do 
		}

		TEST_METHOD(twoStatesTest)
		{
			manager.setState(MockState1::getInstance());

			manager.processInput(2);

			Assert::IsTrue(manager.getState() == MockState2::getInstance()); //state is now 2 instead of 1
		}

		TEST_METHOD(getStateName)
		{
			manager.setState(MockState1::getInstance());
			
			Assert::IsTrue("MockState1" == manager.getState()->getName()); //state is now 2 instead of 1
		}

	};
}