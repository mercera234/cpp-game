#pragma once

#include <iostream>
#include <list>
#include <sstream>
#include "Format.h"
#include "curses.h"
#include "Drawable.h"

/*One piece of solid text to be drawn in a window as part of a larger Controllable.*/
class TextPiece : public DrawablePiece
{
protected:
	std::string text;
	Format* fmt = nullptr;
public:
	TextPiece() {}
	TextPiece(Format* fmtIn, const std::string& textIn);
	~TextPiece();
	virtual void draw(WINDOW* win);

	
	//setters/getters

	void setText(const std::string& textIn) { text = textIn; }
	std::string getText() { return text; }
	void setFormat(Format* fmtIn) { fmt = fmtIn; }
};


