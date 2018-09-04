#include "TextPiece.h"
#include <iomanip>

TextPiece::TextPiece(Format* fmtIn, const std::string& textIn)
{
	setFormat(fmtIn);
	setText(textIn);
}

void TextPiece::draw(WINDOW* win)
{
	fmt->draw(win, text);
}

TextPiece::~TextPiece()
{
	if (fmt != nullptr)
	{
		delete fmt;
		fmt = nullptr;
	}
		
}



