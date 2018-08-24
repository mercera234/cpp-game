#include "CppUnitTest.h"
#include "WadDatabase.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace RPGLibTester
{
	//these tests aren't that good since most of the methods interact with the file system
	TEST_CLASS(WadDatabaseTest)
	{
		TEST_METHOD(printBlankTest)
		{
			WadDatabase database;

			database.print();

			Assert::IsTrue(true);
		}

		TEST_METHOD(getActorsTest)
		{
			WadDatabase database;

			std::ifstream is;
			database.loadActorsFromTextFile(is);
			
			Assert::IsTrue(true);
		}

	};
}