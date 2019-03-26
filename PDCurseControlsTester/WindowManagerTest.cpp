#include "CppUnitTest.h"
#include "WindowManager.h"
#include "TUI.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Microsoft { namespace VisualStudio { namespace CppUnitTestFramework {
			template<>
			static std::wstring ToString<WindowManager>(const class WindowManager& t)
			{
				return L"WindowManager";
			}
}}}


namespace PDCurseControlsTester
{
	TEST_CLASS(WindowManagerTest)
	{
		TUI tui;
		WindowManager winMgr;
		

		TEST_METHOD(getCountTest)
		{
			winMgr.newWin(10, 3, 0, 0);
			winMgr.newWin(4, 9, 2, 4);
			winMgr.newWin(5, 5, 7, 10);

			Assert::AreEqual(3, winMgr.getCount());
		}


		TEST_METHOD(newWinTest)
		{
			WINDOW* win = winMgr.newWin(3, 2, 0, 0);
			Assert::AreNotEqual((int)nullptr, (int)win);
		}

		TEST_METHOD(deriveWinTest)
		{
			int begY = 1;
			int begX = 3;
			WINDOW* win = winMgr.newWin(6, 10, begY, begX);
			Rect r(3, 2, Pos(0, 0));
			WINDOW* win2 = winMgr.deriveWin(win, r);

			Assert::AreEqual(begY, getbegy(win2));
			Assert::AreEqual(begX, getbegx(win2));
		}

		TEST_METHOD(checkWinTest)
		{
			int rows = 3;
			int cols = 9;
			int y = 4;
			int x = 7;
			WINDOW* win = winMgr.newWin(rows, cols, y, x);
			Assert::AreEqual(rows, win->_maxy);
			Assert::AreEqual(cols, win->_maxx);
			Assert::AreEqual(y, win->_begy);
			Assert::AreEqual(x, win->_begx);
		}

		TEST_METHOD(delWinTest)
		{
			WINDOW* win = winMgr.newWin(3, 2, 0, 0);

			winMgr.delWin(win);

			Assert::AreEqual(0, winMgr.getCount());
		}

	};
}