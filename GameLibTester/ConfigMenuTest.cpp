#include "CppUnitTest.h"
#include "ConfigMenu.h"
#include "defaults.h"
#include "GameInput.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace GameLibTester
{
	TEST_CLASS(ConfigMenuTest)
	{
		TUI tui;
		ConfigMenu menu;
		ResourceManager rm;

		TEST_METHOD_INITIALIZE(start)
		{
			auto& inputs = rm.getInputManager().getInputs();
			loadHardCodedInputs(inputs);
			setupDefaultGameInputs(inputs);
			menu.setResourceManager(&rm);
		}


		TEST_METHOD(getResourceManagerTest)
		{
			Assert::AreEqual((int)&rm, (int)menu.getResourceManager());
		}

		TEST_METHOD(setWindowTest)
		{
			WINDOW* win = TUI::winMgr.newWin(5, 10, 3, 4);
			menu.setWindow(win);

			Assert::AreEqual((int)win, (int)menu.getWindow());
		}

		TEST_METHOD(setEditStateTest)
		{
			menu.processInput(GameInput::OK_INPUT); //only ok input can be used here

			Assert::IsTrue(menu.getEditState());
		}

		TEST_METHOD(setEditStateAfterKeyChangeTest)
		{
			menu.processInput(GameInput::OK_INPUT);
			menu.processInput('0');//need to use a key here that isn't loaded by the routine above

			Assert::IsFalse(menu.getEditState());
		}

		TEST_METHOD(changeKeyTest)
		{
			menu.processInput(GameInput::OK_INPUT); //without using directional keys, the ok input will be the current item

			int newKey = 'w';
			menu.processInput(newKey); 

			auto& inputs = rm.getInputManager().getInputs();

			Input input = inputs[newKey];
			Assert::AreEqual(input.name.c_str(), "ok");
		}

		TEST_METHOD(changeDifferentKeyTest)
		{
			menu.processInput(GameInput::DOWN_INPUT);
			menu.processInput(GameInput::DOWN_INPUT);
			menu.processInput(GameInput::OK_INPUT); //without using directional keys, the ok input will be the current item

			int newKey = 'w';
			menu.processInput(newKey);

			auto& inputs = rm.getInputManager().getInputs();

			Input input = inputs[newKey];
			Assert::AreEqual(input.name.c_str(), "openmenu");
		}

		TEST_METHOD(cancelKeyChangeTest)
		{
			menu.processInput(GameInput::OK_INPUT); //without using directional keys, the ok input will be the current item
			int cancelKey = 'x';
			menu.processInput(cancelKey);
			
			Assert::IsFalse(menu.getEditState());
		}

		TEST_METHOD(illegalKeyChangeTest)
		{
			menu.processInput(GameInput::OK_INPUT); //without using directional keys, the ok input will be the current item
			int illegalKey = 0;
			menu.processInput(illegalKey);

			Assert::IsTrue(menu.getEditState()); //edit state is still on
		}

		TEST_METHOD(drawTest)
		{
			WINDOW* win = TUI::winMgr.newWin(10, 10, 0, 0);
			menu.setWindow(win);
			menu.draw();

			char buf[10];
			mvwinnstr(win, 0, 0, buf, 8);
			
			Assert::AreEqual("->ok = c", buf);
		}
	};
}