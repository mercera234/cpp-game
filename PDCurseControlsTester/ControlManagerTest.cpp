#include "CppUnitTest.h"
#include "ControlManager.h"
#include "MockControl.h"
#include "mockCallbacks.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PDCurseControlsTester
{
	

	TEST_CLASS(ControlManagerTest)
	{
		ControlManager cm;

		TEST_METHOD_INITIALIZE(begin)
		{
			cm.setCaller(this);
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

		TEST_METHOD(popControlTest)
		{
			MockControl m;
			cm.registerControl(&m, NULL, NULL);
			cm.popControl();

			Assert::AreEqual(0, cm.getControlCount()); //one was registered and then popped
		}

		TEST_METHOD(popFocusedControlTest)
		{
			MockControl m;
			MockControl m1;
			cm.registerControl(&m, NULL, NULL);
			cm.registerControl(&m1, NULL, NULL);
			cm.setFocus(&m1);
			cm.popControl(); //m1 is popped, transferring focus to the next control, which is m
			
			Controllable* c = cm.getFocus();

			Assert::AreEqual((int)c, (int)&m); //one was registered and then popped
		}

		TEST_METHOD(registerShortcutKeyTest)
		{
			cm.registerShortcutKey('q', mockCallBack);
			
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
		
			Controllable* c = cm.getFocus();

			Assert::IsTrue(c == nullptr);
		}

		TEST_METHOD(setFocusTest)
		{
			MockControl m;
			MockControl m1;
			cm.registerControl(&m, NULL, NULL); 
			cm.registerControl(&m1, NULL, NULL); 

			cm.setFocus(&m1);
			Controllable* c = cm.getFocus();

			bool pointersMatch = &m1 == c;
			Assert::IsTrue(pointersMatch);
		}

		TEST_METHOD(illegalFocusTest)
		{
			MockControl m;
			MockControl m1;
			cm.registerControl(&m, NULL, NULL);
			cm.registerControl(&m1, NULL, NULL);

			cm.setFocus(&m1); //giving m1 focus, should unset focus from m
			
			Assert::IsFalse(m.isFocused());
		}

		TEST_METHOD(shutdownTest)
		{
			MockControl m;
			MockControl m1;
			cm.registerControl(&m, NULL, NULL);
			cm.setActive(false);
			
			Assert::IsTrue(cm.shutdown()); 
		}

		TEST_METHOD(illegalShutdownTest)
		{
			MockControl m;
			MockControl m1;
			cm.registerControl(&m, NULL, NULL);
			
			Assert::IsFalse(cm.shutdown());  //won't work since still active
		}

		TEST_METHOD(handleGlobalInputTest)
		{
			int value = 679;
			cm.registerShortcutKey(value, setCyclicKeyInControlManager); //just a callback we can use to prove that input was handled successfully
			
			cm.handleInput(value);

			Assert::AreEqual(value, (int)cm.getCycleKey());  
		}

		TEST_METHOD(handleGlobalCyclicInputTest)
		{
			int value = '\t';
			cm.setCycleKey(value); //should be default anyway, but just being explicit
			
			Assert::IsTrue(cm.handleInput(value));
		}

		TEST_METHOD(handleNonGlobalInputWithNoControlsTest)
		{
			Assert::IsFalse(cm.handleInput('5'));
		}

		TEST_METHOD(handleModalInputTest)
		{
			MockControl m; 
			cm.registerControl(&m, KEY_LISTENER, mockCallBack);
			int value = 89;
			m.setModal(true); //it is modal by default, but we set it here explicitly to be clear about what is going on
			cm.setFocus(&m);

			Assert::IsTrue(cm.handleInput(value));
		}

		TEST_METHOD(handleInputTest)
		{
			MockControl m;
			cm.setCaller(&m);

			cm.registerControl(&m, KEY_LISTENER, setXinMockControl);
			int value = 9001;
			m.setModal(false);
			cm.setFocus(&m);

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

			cm.registerControl(&m1, KEY_LISTENER, mockCallBack);
			cm.registerControl(&m2, KEY_LISTENER, mockCallBack);
			cm.registerControl(&m3, KEY_LISTENER, mockCallBack);
			cm.registerControl(&m4, KEY_LISTENER, mockCallBack);
			cm.registerControl(&m5, KEY_LISTENER, mockCallBack);

			//m5 will be at top of stack, so we will attempt to move m3 to top
			cm.moveControlToTop(&m3);
			//resulting order will be 12453

			Assert::AreEqual((int)&m3, (int)cm.getTopControl());
		}
	};
}