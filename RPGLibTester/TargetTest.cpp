#include "CppUnitTest.h"
#include "Target.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace RPGLibTester
{
	TEST_CLASS(TargetSchemeTest)
	{
		TargetScheme scheme;

		TEST_METHOD(setSingleOnly)
		{			
			scheme.set.set(TargetSet::SINGLE);
			
			Assert::AreEqual(1, (int)scheme.set.count());
		}

		TEST_METHOD(setSideAndSingle)
		{ 
			scheme.set |= TargetSet::SIDE | TargetSet::SINGLE;

			Assert::AreEqual(2, (int)scheme.set.count());
		}

		TEST_METHOD(setAllBits)
		{
			//set all bits 
			scheme.set |= TargetSet::SIDE | TargetSet::SINGLE | TargetSet::ALL;

			Assert::AreEqual(3, (int)scheme.set.count());
		}
	};
}