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

		TEST_METHOD(intCommandTest)
		{
			SimpleCommand<MockApplication> simpleCmd;
			MockApplication app;

			simpleCmd.setReceiver(&app);
			simpleCmd.setOneArgAction(&MockApplication::operation2); //must use method that matches signature of Action

															  //executes using polymorphism and virtual method execute
			Command* cmd;
			cmd = &simpleCmd;
			int x = cmd->execute(678);

			Assert::AreEqual(x, app.x);
		}
	};
}