#include "CppUnitTest.h"
#include "GridMenu.h"
#include "MockMenuItem.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PDCurseControlsTester
{
	TEST_CLASS(GridMenu_inputTest)
	{
		TUI tui;
		WINDOW* stdscrCpy = nullptr;
		GridMenu menu;

		TEST_METHOD_INITIALIZE(start)
		{
			stdscrCpy = dupwin(stdscr);
			menu.setWindow(stdscrCpy);
			menu.resetItems(2, 2);
			
			menu.setItem(new MockMenuItem, 0, 0);
			menu.setItem(new MockMenuItem, 0, 1);
			menu.setItem(new MockMenuItem, 1, 0);
			menu.setItem(new MockMenuItem, 1, 1);

			menu.post(true);
		}

		TEST_METHOD_CLEANUP(end)
		{
			delwin(stdscrCpy);
		}

		TEST_METHOD(processInputTest)
		{
			GridMenu menu;
			Assert::AreEqual((int)ExitCode::NOT_HANDLED, (int)processInput(menu, 5));
		}

		TEST_METHOD(processRightMovementInputTest)
		{
			menu.setCurrentItem(0);

			menu.setInput(REQ_RIGHT_ITEM);
			menu.processInput();

			Assert::AreEqual(1, (int)menu.getCurrentItem()->index);
		}

		TEST_METHOD(processLeftMovementInputTest)
		{
			menu.setCurrentItem(1);

			processInput(menu, REQ_LEFT_ITEM);

			Assert::AreEqual(0, (int)menu.getCurrentItem()->index);
		}

		TEST_METHOD(processDownMovementInputTest)
		{
			menu.setCurrentItem(0);

			processInput(menu, REQ_DOWN_ITEM);

			Assert::AreEqual(2, (int)menu.getCurrentItem()->index);
		}

		TEST_METHOD(processUpMovementInputTest)
		{
			menu.setCurrentItem(3);

			processInput(menu, REQ_UP_ITEM);

			Assert::AreEqual(1, (int)menu.getCurrentItem()->index);
		}
	};
}