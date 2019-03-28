#include "CppUnitTest.h"
#include "DrawBoard.h"
#include "Drawable.h"
#include "TUI.h"
#include "TextPiece.h"
#include "LineFormat.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PDCurseControlsTester
{
	TEST_CLASS(DrawBoardTest)
	{
		TUI tui;
		DrawBoard<DrawablePiece> board;

		TEST_METHOD(addPieceTest)
		{
			board.addPiece(new TextPiece());
			
			Assert::AreEqual(1, board.getPieceCount());			
		}

		TEST_METHOD(drawTest)
		{
			board.addPiece(new TextPiece(new LineFormat(0, Justf::LEFT), "Words"));
			board.setWindow(stdscr);

			board.draw();
			
			char s[10];

			mvwinnstr(stdscr, 0, 0, s, 5);
			Assert::AreEqual("Words", s);
		}
	};
}