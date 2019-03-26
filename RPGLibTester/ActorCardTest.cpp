#include "CppUnitTest.h"
#include "ActorCard.h"
#include "actor_helper.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace RPGLibTester
{
	TEST_CLASS(ActorCardTest)
	{
		//ActorCard card;
		TEST_METHOD(ctorTest)
		{
			Actor a;
			initDefaultActor(a);
			
			ActorCard card(&a, 0);
			
			Assert::AreEqual((int)&a, (int)card.getActor());
		}


	};
}