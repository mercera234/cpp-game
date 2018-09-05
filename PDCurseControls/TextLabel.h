#pragma once
#include "Controllable.h"
#include "Style.h"
#include "Format.h"
#include "Direction.h"
#include <iostream>

/*A label of text. The difference between this and a TextPiece is 
that the label has its own window. */
class TextLabel : public Controllable
{
private:
	std::string text;
	Format* format = nullptr;
public:
	TextLabel();
	TextLabel(WINDOW* win, const std::string& text);
	~TextLabel();

	void draw();

	//getters/setters
	void setText(const std::string& text) { this->text = text; }
	std::string getText() { return text; }
	void setFormat(Format* fmtIn) { format = fmtIn; }
	Format* getFormat() { return format; }
};


