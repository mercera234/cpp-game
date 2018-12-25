#include "CppUnitTest.h"
#include "TextPiece.h"
#include "TUI.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PDCurseControlsTester
{
	TEST_CLASS(TextPieceTest)
	{
		TUI tui;
		TEST_METHOD(setTextTest)
		{
			TextPiece piece;
			std::string text = "words";
			piece.setText(text);

			Assert::AreEqual(text.c_str(), piece.getText().c_str());
		}

		TEST_METHOD(ctorTest)
		{
			std::string text = "text";
			
			TextPiece piece(new PosFormat(0, 0), text);

			Assert::AreEqual(text.c_str(), piece.getText().c_str());
		}

		TEST_METHOD(drawTest)
		{
			std::string text = "text";
			int y = 4, x = 5;
			TextPiece piece(new PosFormat(y, x), text);
			piece.draw(stdscr);

			char str[10];
			mvwinnstr(stdscr, y, x, str, text.length());

			Assert::AreEqual(text.c_str(), str);
		}


	};
}