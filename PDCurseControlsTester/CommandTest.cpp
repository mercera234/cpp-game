#include "CppUnitTest.h"
#include "SimpleCommand.h"
#include "MockApplication.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PDCurseControlsTester
{
	TEST_CLASS(CommandTest)
	{


		TEST_METHOD(simpleCommandTest)
		{
			SimpleCommand<MockApplication> simpleCmd;
			MockApplication app;
			
			simpleCmd.setReceiver(&app);
			simpleCmd.setAction(&MockApplication::operation); //must use method that matches signature of Action

			//executes using polymorphism and virtual method execute
			Command* cmd;
			cmd = &simpleCmd;
			cmd->execute();

			Assert::AreEqual(234, app.x);
		}

	};
}