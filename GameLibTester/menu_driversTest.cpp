#include "CppUnitTest.h"
#include "defaults.h"
#include "MockMenuItem.h"
#include "menu_drivers.h"
#include "GameInput.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace GameLibTester
{
	TEST_CLASS(menu_driversTest)
	{
		TUI tui;
		ResourceManager rm;
		GridMenu menu;
		const int rows = 3;
		const int cols = 4;
		const int startNdx = 5;

		TEST_METHOD_INITIALIZE(start)
		{
			auto& inputs = rm.getInputManager().getInputs();
			loadHardCodedInputs(inputs);
			setupDefaultGameInputs(inputs);
			
			//create a menu for testing
			menu.resetItems(rows, cols);
			menu.setWindow(dupwin(stdscr));
			
			for (int i = 0; i < rows * cols; i++)
			{
				MockMenuItem* item = new MockMenuItem();
				item->index = i;
				item->selectable = true;
				menu.setItem(item);
			}
			menu.setCurrentItem(5); //default item will be in middle so we have some movement options
			menu.post(true);
		}



		TEST_METHOD(menuDriverDownInputTest)
		{
			menuDriver(GameInput::DOWN_INPUT, &menu); //started at 5
			MenuItem* item = menu.getCurrentItem();
			
			Assert::AreEqual(startNdx + cols, (int)item->index);
		}

		TEST_METHOD(menuDriverUpInputTest)
		{
			menuDriver(GameInput::UP_INPUT, &menu); //started at 5
			MenuItem* item = menu.getCurrentItem();

			Assert::AreEqual(startNdx - cols, (int)item->index);
		}

		TEST_METHOD(menuDriverLeftInputTest)
		{
			menuDriver(GameInput::LEFT_INPUT, &menu); //started at 5
			MenuItem* item = menu.getCurrentItem();

			Assert::AreEqual(startNdx - 1, (int)item->index);
		}

		TEST_METHOD(menuDriverRightInputTest)
		{
			menuDriver(GameInput::RIGHT_INPUT, &menu); //started at 5
			MenuItem* item = menu.getCurrentItem();

			Assert::AreEqual(startNdx + 1, (int)item->index);
		}

		TEST_METHOD(menuDriverOkInputTest)
		{
			MenuItem* item = menuDriver(GameInput::OK_INPUT, &menu); //started at 5

			Assert::AreEqual(startNdx, (int)item->index);
		}
	};
}