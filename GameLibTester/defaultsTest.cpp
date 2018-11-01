#include "CppUnitTest.h"
#include "defaults.h"
#include "GameInput.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace GameLibTester
{
	TEST_CLASS(defaultsTest)
	{
		ResourceManager rm;
		TEST_METHOD(setupDefaultDataKeysTest)
		{
			setupDefaultDataKeys(rm);
			Assert::AreEqual(9999999, rm.getData().retrieveIntData(GOLD$).getMax());
		}

		TEST_METHOD(defaultGameInputsNothingAddedTest)
		{
			std::map<int, Input> inputs;
			setupDefaultGameInputs(inputs); //adds 3 default secrets so far

			Assert::AreEqual(3, (int)inputs.size());
		}

		TEST_METHOD(defaultGameInputsTest)
		{
			std::map<int, Input> inputs;
			int keyCode = 'c';
			inputs.insert(std::make_pair(keyCode, Input("ok", -1)));

			setupDefaultGameInputs(inputs);
			
			Assert::AreEqual((int)GameInput::OK_INPUT, inputs[keyCode].code);
		}

		//Not currently testable. No way to simulate input TODO
		//TEST_METHOD(getInputTest)
		//{
		//	getInput(rm);
		//}

		//Not currently testable. How to simulate without reading data from filesystem? TODO
		//TEST_METHOD(loadDataFilesTest)
		//{
		//	
		//}
	};
}