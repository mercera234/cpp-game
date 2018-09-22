#include "CppUnitTest.h"
#include "FreeMovementProcessor.h"
#include "Image.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace RPGLibTester
{
	TEST_CLASS(FreeMovementProcessorTest)
	{
		TUI tui;
		FreeMovementProcessor fmp;
		Image img;
		int y = 0;
		int x = 0;

		TEST_METHOD_INITIALIZE(startTUI)
		{
			img.setWindow(stdscr);
			img.setDimensions(5, 5);
			fmp.setMoveControl(&img);
			fmp.setCursor(&y, &x);
		}

		TEST_METHOD(setViewMode)
		{
			ViewMode mode = ViewMode::CENTER;
			fmp.setViewMode(mode);

			Assert::IsTrue(mode == fmp.getViewMode());
		}

		TEST_METHOD(rightKey)
		{
			//cursor starts out at 0,0
			fmp.processMovementInput(KEY_RIGHT);

			Assert::AreEqual(1, x);
		}

		TEST_METHOD(leftKey)
		{
			//cursor starts out at 0,0
			x = 3;
			fmp.processMovementInput(KEY_LEFT);

			Assert::AreEqual(2, x);
		}

		TEST_METHOD(downKey)
		{
			//cursor starts out at 0,0
			fmp.processMovementInput(KEY_DOWN);

			Assert::AreEqual(1, y);
		}

		TEST_METHOD(upKey)
		{
			y = 3;
			fmp.processMovementInput(KEY_UP);

			Assert::AreEqual(2, y);
		}


		TEST_METHOD(rightKeyOutOfBounds)
		{
			x = 4;
			fmp.processMovementInput(KEY_RIGHT);

			Assert::AreEqual(4, x);
		}

		
		TEST_METHOD(leftKeyOutOfBounds)
		{
			//cursor starts out at 0,0
			fmp.processMovementInput(KEY_LEFT);

			Assert::AreEqual(0, x); //x should be unchanged
		}

		TEST_METHOD(downKeyOutOfBounds)
		{
			y = 4;
			fmp.processMovementInput(KEY_DOWN);

			Assert::AreEqual(4, y);
		}

		TEST_METHOD(upKeyOutOfBounds)
		{
			//cursor starts out at 0,0
			fmp.processMovementInput(KEY_UP);

			Assert::AreEqual(0, y);
		}


		TEST_METHOD(pageDown)
		{
			int rows = getmaxy(stdscr) * 3;
			int cols = getmaxx(stdscr) * 2;
			img.setDimensions(rows, cols);

			//cursor starts out at 0,0
			fmp.processMovementInput(KEY_PGDN);

			Assert::AreEqual(img.getVisibleRows() - 1, y); //y should be 1 less than visible rows
		}

		TEST_METHOD(pageUp)
		{
			int rows = getmaxy(stdscr) * 3;
			int cols = getmaxx(stdscr) * 2;
			img.setDimensions(rows, cols);

			int offset = 10;
			y = img.getVisibleRows() + offset;
			
			fmp.processMovementInput(KEY_PGUP);

			Assert::AreEqual(offset, y); 
		}

		TEST_METHOD(pageLeft)
		{
			int rows = getmaxy(stdscr) * 3;
			int cols = getmaxx(stdscr) * 2;
			img.setDimensions(rows, cols);

			int offset = 22;
			x = img.getVisibleCols() + offset;

			fmp.processMovementInput(CTL_PGUP);

			Assert::AreEqual(offset, x); 
		}

		TEST_METHOD(pageRight)
		{
			int rows = getmaxy(stdscr) * 3;
			int cols = getmaxx(stdscr) * 2;
			img.setDimensions(rows, cols);

			//cursor starts out at 0,0
			fmp.processMovementInput(CTL_PGDN);

			Assert::AreEqual(img.getVisibleCols() - 1, x); 
		}


		TEST_METHOD(pageDownOutOfBounds)
		{
			//cursor starts out at 0,0
			fmp.processMovementInput(KEY_PGDN);

			Assert::AreEqual(4, y); //y should be one less than height of image
		}

		
		TEST_METHOD(pageUpOutOfBounds)
		{
			y = 1;
			fmp.processMovementInput(KEY_PGUP);

			Assert::AreEqual(0, y);
		}

		TEST_METHOD(pageRightOutOfBounds)
		{
			x = 3;
			fmp.processMovementInput(CTL_PGDN);

			Assert::AreEqual(4, x); 
		}

		TEST_METHOD(pageLeftOutOfBounds)
		{
			x = 3;
			fmp.processMovementInput(CTL_PGUP);

			Assert::AreEqual(0, x);
		}

		TEST_METHOD(homeKey)
		{
			y = 1;
			x = 4;
			fmp.processMovementInput(KEY_HOME);

			Assert::AreEqual(0, x);
			Assert::AreEqual(1, y);
		}

		TEST_METHOD(endKey)
		{
			y = 3;
			x = 1;
			fmp.processMovementInput(KEY_END);

			Assert::AreEqual(4, x);
			Assert::AreEqual(3, y);
		}

		TEST_METHOD(ctrlHomeKey)
		{
			y = 1;
			x = 4;
			fmp.processMovementInput(CTL_HOME);

			Assert::AreEqual(0, x);
			Assert::AreEqual(0, y);
		}

		TEST_METHOD(ctrlEndKey)
		{
			y = 3;
			x = 3;
			fmp.processMovementInput(CTL_END);

			Assert::AreEqual(4, x);
			Assert::AreEqual(4, y);
		}
	};
}