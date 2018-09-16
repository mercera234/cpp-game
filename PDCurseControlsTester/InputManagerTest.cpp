#include "CppUnitTest.h"
#include <algorithm>
#include "InputManager.h"
#include "Input.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PDCurseControlsTester
{
	TEST_CLASS(InputManagerTest)
	{
		TEST_METHOD(fill)
		{
			InputManager<int> mgr;
			mgr.fill(-1);

			int& firstKey = mgr.getInput(0);
			int& lastKey = mgr.getInput(MAX_KEY_CODES - 1);

			Assert::AreEqual(firstKey, lastKey);
		}

		TEST_METHOD(fill2)
		{
			InputManager<int> mgr;

			mgr.fill(-1);

			int& key = mgr.getInput(45); //could use any number between 0 and max_key_codes

			Assert::AreEqual(-1, key);
		}

		TEST_METHOD(setIntInputTest)
		{
			InputManager<int> mgr;

			int input = 6;
			int key = 'y';
			mgr.setInput(key, input);

			Assert::AreEqual(input, mgr.getInput(key));
		}

		TEST_METHOD(fillInputTest)
		{
			InputManager<Input> mgr;

			int nullAction = -1;
			mgr.fill(Input("", nullAction, false));
			Input& i = mgr.getInput(MAX_KEY_CODES - 5);

			Assert::AreEqual(nullAction, i.code);
		}

		TEST_METHOD(setInputTest)
		{
			InputManager<Input> mgr;

			int gameInput = 1;
			mgr.setInput('c', Input("OK", gameInput));
			Input& i = mgr.getInput('c');

			Assert::AreEqual(gameInput, i.code);
		}

		TEST_METHOD(fillInputPtrTest)
		{
			InputManager<Input*> mgr;

			mgr.fill(nullptr);
			Input* i = mgr.getInput(MAX_KEY_CODES - 5);

			Assert::AreEqual((int)nullptr, (int)i);
		}
	};
}