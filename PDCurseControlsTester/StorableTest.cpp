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
			ms.load(ifs);

			Assert::IsTrue(ms.loadOccurred);
		}

		TEST_METHOD(saveTest)
		{
			MockStorable ms;

			std::ofstream ofs;
			ms.save(ofs);

			Assert::IsTrue(ms.saveOccurred);
		}
	};
}