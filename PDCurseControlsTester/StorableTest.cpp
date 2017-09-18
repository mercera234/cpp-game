#include "CppUnitTest.h"
#include "MockStorable.h"
#include <fstream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PDCurseControlsTester
{
	TEST_CLASS(StorableTest)
	{
		TEST_METHOD(loadTest)
		{
			MockStorable ms;

			std::ifstream ifs;
			
			Assert::AreEqual(600, ms.load(ifs));
		}

		TEST_METHOD(saveTest)
		{
			MockStorable ms;

			std::ofstream ofs;

			Assert::AreEqual(600, ms.save(ofs));
		}
	};
}