#include "TextBoard.h"

TextBoard::TextBoard()
{
}

TextBoard::~TextBoard()
{
}

void TextBoard::addPiece(TextPiece* piece)
{
	pieces.push_back(piece);
}

void TextBoard::draw()
{
	wclear(win);
	for each (TextPiece* piece in pieces)
	{
		piece->draw(win);
	}
	wnoutrefresh(win);
}


