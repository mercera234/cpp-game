#include "CppUnitTest.h"
#include <algorithm>
#include "InputManager.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PDCurseControlsTester
{
	TEST_CLASS(InputManagerTest)
	{
		TEST_METHOD(constructorFillTest)
		{
			InputManager mgr;

			int firstKey = mgr.getInput(0);
			int lastKey = mgr.getInput(MAX_KEY_CODES);

			Assert::AreEqual(firstKey, lastKey);
		}

		TEST_METHOD(constructorFillWithNoInputTest)
		{
			InputManager mgr;

			int key = mgr.getInput(45); //could use any number between 0 and max_key_codes

			Assert::AreEqual(NO_INPUT, key);
		}

		TEST_METHOD(setInputTest)
		{
			InputManager mgr;

			int input = 6;
			int key = 'y';
			mgr.setInput(key, input);

			Assert::AreEqual(input, mgr.getInput(key));
		}
	};
}