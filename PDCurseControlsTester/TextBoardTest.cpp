#include "CppUnitTest.h"
#include "TextBoard.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PDCurseControlsTester
{
	TEST_CLASS(TextBoardTest)
	{
		TUI tui;
		
		TEST_METHOD(addPieceTest)
		{
			TextBoard board;
			board.addPiece(new TextPiece());

			Assert::AreEqual(1, board.getPieceCount());
		}

		

		
	};
}