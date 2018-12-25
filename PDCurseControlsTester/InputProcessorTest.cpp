#include "CppUnitTest.h"
#include "MockInputProcessor.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PDCurseControlsTester
{
	TEST_CLASS(InputProcessorTest)
	{
		MockInputProcessor processor;

		TEST_METHOD(processNoInputTest)
		{
			processor.processInput(); //value of x will be undefined, but exitcode should be the same regardless

			Assert::AreEqual((int)ExitCode::HANDLED, (int)processor.getExitCode());
		}

		TEST_METHOD(processInputTest)
		{
			int input = 9;
			int origX = processor.getX();
			processor.setInput(input);
			processor.processInput(); //value of x will be undefined, but exitcode should be the same regardless

			Assert::AreEqual(input + origX, processor.getX());
		}

		TEST_METHOD(processBadInputTest)
		{
			int input = -80;
			int origX = processor.getX();
			processor.setInput(input);
			processor.processInput(); //value of x will be undefined, but exitcode should be the same regardless

			Assert::AreEqual((int)ExitCode::NOT_HANDLED, (int)processor.getExitCode());
		}

		TEST_METHOD(processInputFunctionTest)
		{
			int x = processor.getX();
			int input = 35;
			ExitCode code = processInput(processor, input);

			Assert::AreEqual(input + x, processor.getX());
			Assert::AreEqual((int)ExitCode::HANDLED, (int)code);
		}
	};
}