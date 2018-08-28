#include "TextBoard.h"

void TextPiece::draw(WINDOW* win)
{
	fmt->draw(win, text);
}

void TextPiece::setText(const std::string& textIn)
{
	text = textIn;
}

std::string TextPiece::getText()
{
	return text;
}


void TextPiece::setFormat(Format* fmtIn)
{
	fmt = fmtIn;
}



void MacroTextPiece::expandTextPieces()
{
	text = ""; //reset text
	for each (TextPiece* piece in pieces)
	{
		text.append(piece->getText());
	}
}

void MacroTextPiece::appendPiece(TextPiece* piece)
{
	pieces.push_back(piece);
}

void MacroTextPiece::draw(WINDOW* win)
{
	expandTextPieces();
	fmt->draw(win, text);
}

std::string MacroTextPiece::getText()
{
	expandTextPieces();
	return text;
}

MacroTextPiece::~MacroTextPiece()
{
}


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
	for each (TextPiece* piece in pieces)
	{
		piece->draw(win);
	}
	wnoutrefresh(win);
}