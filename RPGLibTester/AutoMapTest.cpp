#include "CppUnitTest.h"
#include "AutoMap.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace RPGLibTester
{
	TEST_CLASS(AutoMapTest)
	{
		TUI tui;
		AutoMap autoMap;

		TEST_METHOD(ctorTest)
		{			
			Assert::IsTrue(autoMap.getCurrMap() == nullptr);
		}

		TEST_METHOD(setCurrMapTest)
		{
			MegaMap aMap;
			autoMap.setCurrMap(&aMap);

			Assert::IsTrue(autoMap.getCurrMap() == &aMap);
		}




	};
}