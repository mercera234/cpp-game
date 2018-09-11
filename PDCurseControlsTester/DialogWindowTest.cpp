#include "CppUnitTest.h"
#include "DialogWindow.h"
#include "MockControl.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PDCurseControlsTester
{
	TEST_CLASS(DialogWindowTest)
	{
		TUI tui;
		TEST_METHOD_INITIALIZE(startTUI)
		{
			tui.init();
		}

		TEST_METHOD_CLEANUP(stopTUI)
		{
			tui.shutdown();
		}

		TEST_METHOD(ctorTest)
		{
			DialogWindow dialog;
			Assert::IsTrue(dialog.isModal());
		}

		TEST_METHOD(setWindowTest)
		{
			DialogWindow dialog;
			WINDOW* win = newwin(3,3,1,1);
			dialog.setWindow(win);
			Assert::IsTrue(dialog.getWindow() == win);
		}

		TEST_METHOD(getInnerWindowTest)
		{
			DialogWindow dialog;
			int rows = 6;
			int cols = 7;
			WINDOW* win = newwin(rows, cols, 1, 1);
			dialog.setWindow(win);
			WINDOW* iw = dialog.getInnerWin();
			Assert::AreEqual(rows - 2, getmaxy(iw));
			Assert::AreEqual(cols - 2, getmaxx(iw));
		}

		TEST_METHOD(setControlTest)
		{
			DialogWindow dialog;
			MockControl* control = new MockControl(); //control will be deleted by the DialogWindow once out of scope
			dialog.setControl(control);
			Assert::IsTrue(dialog.getControl() == control);
		}
	};
}