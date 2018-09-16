#include "CppUnitTest.h"
#include "ControlManager.h"
#include "MockControl.h"
#include "mockCallbacks.h"
#include "SimpleCommand.h"
#include "MockApplication.h"
#include "MockCommand.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PDCurseControlsTester
{
	

	TEST_CLASS(ControlManagerTest)
	{
		ControlManager cm;
		MockApplication app;
		SimpleCommand<MockApplication> cmd;

		TUI tui;
		
		TEST_METHOD_INITIALIZE(begin)
		{
			cm.setCaller(this);
			tui.init();
		}

		TEST_METHOD_CLEANUP(end)
		{
			tui.shutdown();
		}

		TEST_METHOD(registerControlTest)
		{
			MockControl m;
			cm.registerControl(&m, NULL, NULL);

			Assert::AreEqual(1, cm.getControlCount());
		}

		TEST_METHOD(unRegisterControlTest)
		{
			MockControl m;
			cm.registerControl(&m, NULL, NULL);
			cm.unRegisterControl(&m);

			Assert::AreEqual(0, cm.getControlCount());
		}

		TEST_METHOD(reRegisterControlTest)
		{
			MockControl m;
			int value = 3;
			m.setX(value);
			cm.registerControl(&m, NULL, NULL);
			cm.unRegisterControl(&m);
			cm.registerControl(&m, NULL, NULL);

			Assert::AreEqual(1, cm.getControlCount()); //this is to prove that the control is not destroyed once unregistered
			Assert::AreEqual(value, m.getX());
		}

		TEST_METHOD(popControlTest)
		{
			MockControl m;
			cm.registerControl(&m, NULL, NULL);
			cm.popControl();

			Assert::AreEqual(0, cm.getControlCount()); //one was registered and then popped
		}

		TEST_METHOD(popControlReturnsControlTest)
		{
			MockControl m;
			int value = 67;
			m.setX(67);
			cm.registerControl(&m, NULL, NULL);
			Controllable* c = cm.popControl();

			Assert::AreEqual(value, ((MockControl*)c)->getX()); 
		}

		TEST_METHOD(popFocusedControlTest)
		{
			MockControl m;
			MockControl m1;
			cm.registerControl(&m, NULL, NULL);
			cm.registerControl(&m1, NULL, NULL);
			cm.setFocusedControl(&m1);
			cm.popControl(); //m1 is popped, transferring focus to the next control, which is m
			
			Controllable* c = cm.getFocusedControl();

			Assert::AreEqual((int)c, (int)&m); //one was registered and then popped
		}

		TEST_METHOD(registerShortcutKeyTest)
		{
			cmd.setReceiver(&app);
			app.cm = &cm;
			cmd.setAction(&MockApplication::mockCallback);


			cm.registerShortcutKey('q', &cmd);
			
			Assert::AreEqual(1, cm.getKeyAcceleratorCount()); 
		}

		TEST_METHOD(setCyclicKeyTest)
		{
			short key = '8';
			cm.setCycleKey(key);

			Assert::AreEqual(key, cm.getCycleKey()); 
		}

		TEST_METHOD(setRevCyclicKeyTest)
		{
			short key = 36;
			cm.setRevCycleKey(key);

			Assert::AreEqual(key, cm.getRevCycleKey()); 
		}

		TEST_METHOD(setUnauthorizedCyclicKeyTest)
		{
			short key = KEY_MOUSE;
			cm.setCycleKey(key);

			Assert::AreNotEqual(key, cm.getCycleKey()); 
		}

		TEST_METHOD(noFocusedControlsTest)
		{
			MockControl m;
			cm.registerControl(&m, NULL, NULL);
		
			Controllable* c = cm.getFocusedControl();

			Assert::IsTrue(c == nullptr);
		}

		TEST_METHOD(setFocusTest)
		{
			MockControl m;
			MockControl m1;
			cm.registerControl(&m, NULL, NULL); 
			cm.registerControl(&m1, NULL, NULL); 

			cm.setFocusedControl(&m1);
			Controllable* c = cm.getFocusedControl();

			bool pointersMatch = &m1 == c;
			Assert::IsTrue(pointersMatch);
		}

		TEST_METHOD(illegalFocusTest)
		{
			MockControl m;
			MockControl m1;
			cm.registerControl(&m, NULL, NULL);
			cm.registerControl(&m1, NULL, NULL);

			cm.setFocusedControl(&m1); //giving m1 focus, should unset focus from m
			
			Assert::IsFalse(m.isFocused());
		}

		TEST_METHOD(handleGlobalInputTest)
		{
			int value = 679;

			cmd.setReceiver(&app);
			cmd.setAction(&MockApplication::mockCallBack2);

			cm.registerShortcutKey(value, &cmd); //just a callback we can use to prove that input was handled successfully
			
			cm.handleInput(value);

			Assert::IsTrue(ExitCode::HANDLED == cm.getExitCode());
		}

		TEST_METHOD(handleGlobalCyclicInputTest)
		{
			int value = '\t';
			cm.setCycleKey(value); //should be default anyway, but just being explicit
			cm.handleInput(value);

			Assert::IsTrue(cm.getExitCode() == ExitCode::HANDLED);
		}

		TEST_METHOD(handleNonGlobalInputWithNoControlsTest)
		{
			cm.handleInput('5');
			Assert::IsFalse(cm.getExitCode() == ExitCode::HANDLED);
		}

		TEST_METHOD(handleModalInputTest)
		{
			MockControl m; 

			cmd.setReceiver(&app);
			app.cm = &cm;
			cmd.setAction(&MockApplication::mockCallback);

			cm.registerControl(&m, KEY_LISTENER, &cmd);
			int value = 89;
			m.setModal(true); //it is modal by default, but we set it here explicitly to be clear about what is going on
			cm.setFocusedControl(&m);
			cm.handleInput(value);

			Assert::IsTrue(cm.getExitCode() == HANDLED);
		}

		TEST_METHOD(handleInputTest)
		{
			MockControl m;
			cm.setCaller(&m);

			cmd.setReceiver(&app);
			app.cm = &cm;
			cmd.setAction(&MockApplication::setXinMockControl);

			cm.registerControl(&m, KEY_LISTENER, &cmd);
			int value = 9001;
			m.setModal(false);
			cm.setFocusedControl(&m);

			cm.handleInput(value);
			Assert::AreEqual(value, m.getX());
		}

		TEST_METHOD(moveControlToTopTest)
		{
			MockControl m1;
			MockControl m2;
			MockControl m3;
			MockControl m4;
			MockControl m5;

			cmd.setReceiver(&app);
			cmd.setAction(&MockApplication::mockCallback);

			cm.registerControl(&m1, KEY_LISTENER, &cmd);
			cm.registerControl(&m2, KEY_LISTENER, &cmd);
			cm.registerControl(&m3, KEY_LISTENER, &cmd);
			cm.registerControl(&m4, KEY_LISTENER, &cmd);
			cm.registerControl(&m5, KEY_LISTENER, &cmd);

			//m5 will be at top of stack, so we will attempt to move m3 to top
			cm.moveControlToTop(&m3);
			//resulting order will be 12453

			Assert::AreEqual((int)&m3, (int)cm.getTopControl());
		}

		TEST_METHOD(cmdTest)
		{
			MockControl m1;
			m1.setModal(false);

			cmd.setReceiver(&app);
			app.cm = &cm;
			cmd.setAction(&MockApplication::mockCallback);

			cm.registerControl(&m1, KEY_LISTENER, &cmd);

			cm.setFocusedControl(&m1);

			int input = 800;
			cm.handleInput(input);
			Assert::IsTrue(ExitCode::HANDLED == cm.getExitCode());

		}

		//mouse input tests

		TEST_METHOD(mouseInputTest)
		{
			app.cm = &cm;

			MockControl m1;
			m1.setModal(false);

			int y, x;
			y = 4;
			x = 3;

			WINDOW* win = newwin(1, 1, y, x);
			m1.setWindow(win);

			cmd.setReceiver(&app);
			cmd.setAction(&MockApplication::setXinMockControl);
			cm.registerControl(&m1, MOUSE_LISTENER, &cmd);
			cm.setFocusedControl(&m1);

			//preset mouse event to simulate mouse input
			simulateMouseOn(true);
			setMouseEvent(y, x);
			cm.handleInput(KEY_MOUSE);

			Assert::AreEqual(KEY_MOUSE, m1.getX());
		}


		/*Control Managers can be chained, but the application must be referring to the correct instance to process their leaves correctly*/
		TEST_METHOD(controlManagerChainingTest)
		{
			app.cm = &cm;

			cmd.setReceiver(&app);
			cmd.setAction(&MockApplication::setXinMockControl2);
			SimpleCommand<MockApplication> cmd2;
			cmd2.setReceiver(&app);
			cmd2.setAction(&MockApplication::transferControl);

			ControlManager cm2;
			app.childCM = &cm2;
			MockControl m1;

			cm2.registerControl(&m1, KEY_LISTENER, &cmd);
			cm2.setFocusedControl(&m1);

			cm.registerControl(&cm2, KEY_LISTENER, &cmd2);
			cm.setFocusedControl(&cm2);

			int input = 800;
			cm.handleInput(input);
			Assert::AreEqual(input, m1.getX());
		}


		


		//TODO
		//add test to verify that registered controls that turn on the cursor also have them turned off once unregistered
		//since mouse control can be accepted by any component, verify that focus goes back to previously registered component( or never appears to change)
	};
}