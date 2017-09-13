#include "CppUnitTest.h"
#include "ControlManager.h"
#include "MockControl.h"
#include "mockCallbacks.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PDCurseControlsTester
{
	

	TEST_CLASS(ControlManagerTest)
	{
		TEST_METHOD(registerControlTest)
		{
			ControlManager cm(this);

			MockControl m;
			cm.registerControl(&m, NULL, NULL);

			Controllable* c = cm.getFocus();

			bool pointersMatch = &m == c;
			Assert::IsTrue(pointersMatch);
		}

		TEST_METHOD(popControlTest)
		{
			ControlManager cm(this);

			MockControl m;
			cm.registerControl(&m, NULL, NULL);
			cm.popControl();

			Assert::AreEqual(0, cm.getControlCount()); //one was registered and then popped
		}

		TEST_METHOD(registerShortcutKeyTest)
		{
			ControlManager cm(this);
			cm.registerShortcutKey('q', mockCallBack);
			
			Assert::AreEqual(1, cm.getKeyAcceleratorCount()); 
		}

		TEST_METHOD(setCyclicKeyTest)
		{
			ControlManager cm(this);
			short key = '8';
			cm.setCycleKey(key);

			Assert::AreEqual(key, cm.getCycleKey()); 
		}

		TEST_METHOD(setRevCyclicKeyTest)
		{
			ControlManager cm(this);
			short key = 36;
			cm.setRevCycleKey(key);

			Assert::AreEqual(key, cm.getRevCycleKey()); 
		}

		TEST_METHOD(setUnauthorizedCyclicKeyTest)
		{
			ControlManager cm(this);
			short key = KEY_MOUSE;
			cm.setCycleKey(key);

			Assert::AreNotEqual(key, cm.getCycleKey()); 
		}

		TEST_METHOD(getFocusTest)
		{
			ControlManager cm(this);
			
			MockControl m;
			cm.registerControl(&m, NULL, NULL); //this element should have focus since it is registered first
		
			Controllable* c = cm.getFocus();

			bool pointersMatch = &m == c;
			Assert::IsTrue(pointersMatch);
		}

		TEST_METHOD(setFocusTest)
		{
			ControlManager cm(this);

			MockControl m;
			MockControl m1;
			cm.registerControl(&m, NULL, NULL); 
			cm.registerControl(&m1, NULL, NULL); 

			cm.setFocus(&m1);
			Controllable* c = cm.getFocus();

			bool pointersMatch = &m1 == c;
			Assert::IsTrue(pointersMatch);
		}

		TEST_METHOD(shutdownTest)
		{
			ControlManager cm(this);

			MockControl m;
			MockControl m1;
			cm.registerControl(&m, NULL, NULL);
			cm.setActive(false);
			
			Assert::IsTrue(cm.shutdown()); 
		}

		TEST_METHOD(illegalShutdownTest)
		{
			ControlManager cm(this);

			MockControl m;
			MockControl m1;
			cm.registerControl(&m, NULL, NULL);
			//cm.setActive(false);
			
			Assert::IsFalse(cm.shutdown());  //won't work since still active
		}

		TEST_METHOD(handleGlobalInputTest)
		{
			ControlManager cm(this);

			int value = 679;
			cm.registerShortcutKey(value, setCyclicKeyInControlManager); //just a callback we can use to prove that input was handled successfully
			
			cm.handleInput(value);

			Assert::AreEqual(value, (int)cm.getCycleKey());  
		}

		TEST_METHOD(handleGlobalCyclicInputTest)
		{
			ControlManager cm(this);

			int value = '\t';
			cm.setCycleKey(value); //should be default anyway, but just being explicit
			
			Assert::IsTrue(cm.handleInput(value));
		}

		TEST_METHOD(handleNonGlobalInputWithNoControlsTest)
		{
			ControlManager cm(this);
			Assert::IsFalse(cm.handleInput('5'));
		}

		TEST_METHOD(handleModalInputTest)
		{
			ControlManager cm(this);

			MockControl m; 
			cm.registerControl(&m, KEY_LISTENER, mockCallBack);
			int value = 89;
			m.setModal(true); //it is modal by default, but we set it here explicitly to be clear about what is going on

			Assert::IsTrue(cm.handleInput(value));
		}

		TEST_METHOD(handleInputTest)
		{
			MockControl m;
			ControlManager cm(&m);

			cm.registerControl(&m, KEY_LISTENER, setXinMockControl);
			int value = 9001;
			m.setModal(false);

			cm.handleInput(value);
			Assert::AreEqual(value, m.getX());
		}
	};
}