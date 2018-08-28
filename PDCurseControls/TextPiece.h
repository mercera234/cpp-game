#pragma once

#include <iostream>
#include <list>
#include "Format.h"

class TextPiece
{
protected:
	std::string text;
	Format* fmt;
public:
	virtual void draw(WINDOW* win);
	void setText(const std::string& textIn);
	virtual std::string getText();
	void setFormat(Format* fmtIn);
};


class MacroTextPiece : public TextPiece
{
protected:
	std::list<TextPiece*> pieces;

public:
	void expandTextPieces();
	void appendPiece(TextPiece* piece);
	void draw(WINDOW* win);
	std::string MacroTextPiece::getText();
	~MacroTextPiece();
};


