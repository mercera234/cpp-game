#include "CppUnitTest.h"
#include "TextBoard.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PDCurseControlsTester
{
	TEST_CLASS(TextBoardTest)
	{
		TUI tui;
		

	/*	TEST_METHOD(expandIntTest)
		{
			VarTextPiece<int> piece;

			piece.setText("%u");
			piece.setValue(900);

			std::string result = piece.getText();

			Assert::IsTrue(result.compare("900") == 0);
		}

		TEST_METHOD(expandStringTest)
		{
			VarTextPiece<const char*> piece;

			piece.setText("|%s|");
			piece.setValue("a string");

			std::string result = piece.getText();

			Assert::IsTrue(result.compare("|a string|") == 0);
		}*/

	/*	TEST_METHOD(macroStringTest)
		{
			VarTextPiece<int> piece;
			piece.setText("%u");
			piece.setValue(38);

			VarTextPiece<const char*> piece2;
			piece2.setText(" %s.");
			piece2.setValue("letters");


			MacroTextPiece bigPiece;
			bigPiece.appendPiece(&piece);
			bigPiece.appendPiece(&piece2);

			std::string result = bigPiece.getText();

			Assert::IsTrue(result.compare("38 letters.") == 0);
		}*/

		TEST_METHOD(addPieceTest)
		{
			TextBoard board;
			board.addPiece(new TextPiece());

			Assert::AreEqual(1, board.getPieceCount());
		}

		TEST_METHOD(addFormatToPieceTest)
		{
			std::string text = "text";
			TextPiece piece(new PosFormat(0, 0), "text");

			Assert::IsTrue(text.compare(piece.getText()) == 0);
		}

		
	};
}