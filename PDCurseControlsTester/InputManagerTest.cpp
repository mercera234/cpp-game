#include "CppUnitTest.h"
#include "InputManager.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PDCurseControlsTester
{
	TEST_CLASS(InputManagerTest)
	{
		InputManager mgr;
		TEST_METHOD_INITIALIZE(start)
		{
			auto& inputs = mgr.getInputs();
			inputs.insert(std::make_pair(3, Input("Test", 6)));
		}

		TEST_METHOD(getInputTest)
		{
			Assert::AreEqual(6, mgr.getInput(3));
		}

		TEST_METHOD(getUnknownInputTest)
		{
			Assert::AreEqual(-1, mgr.getInput(9));
		}

		TEST_METHOD(useRawTest)
		{
			mgr.setUseRawInput(true);
			int key = 167;
			Assert::AreEqual(key, mgr.getInput(key));
		}
	};
}