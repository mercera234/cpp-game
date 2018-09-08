#include "CppUnitTest.h"
#include "MovementProcessor.h"
#include "MockMovementProcessor.h"
#include "Image.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace RPGLibTester
{
	TEST_CLASS(MovementProcessorTest)
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

		TEST_METHOD(setMoveControlTest)
		{
			MockMovementProcessor mmp;

			Image img;
			mmp.setMoveControl(&img);

			Assert::IsTrue(&img == mmp.getMoveControl());
		}

		TEST_METHOD(setViewModeTest)
		{
			MockMovementProcessor mmp;
			//all of this must be done prior to setting view mode
			Image img(1,1,stdscr);
			mmp.setMoveControl(&img);
			int y = 0;
			int x = 0;
			mmp.setCursor(&y, &x);

			ViewMode mode = ViewMode::CENTER;
			mmp.setViewMode(mode);

			Assert::IsTrue(mode == mmp.getViewMode());
		}

		TEST_METHOD(processMovementInputTest)
		{
			MockMovementProcessor mmp;

			Image img(5, 5, stdscr);
			mmp.setMoveControl(&img);
			int y = 0;
			int x = 0;
			mmp.setCursor(&y, &x);

			mmp.processMovementInput(KEY_RIGHT);

			Assert::AreEqual(1, x);
		}

		TEST_METHOD(checkReturnValTest)
		{
			MockMovementProcessor mmp;

			int rows = 3;
			int cols = 6;
			Image img(rows, cols, stdscr);
			mmp.setMoveControl(&img);
			int y = 0;
			int x = 0;
			mmp.setCursor(&y, &x);

			MovementChain mc = mmp.processMovementInput(CTL_END);

			Movement& move1 = mc.moves[0];
			Movement& move2 = mc.moves[1];

			Assert::AreEqual(cols, move1.magnitude);
			Assert::AreEqual(rows, move2.magnitude);
		}
	};
}