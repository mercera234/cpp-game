#pragma once
#include "TextPiece.h"
#include "Controllable.h"
#include "Direction.h"
#include "Style.h"
#include <list>

class TextBoard : public Controllable
{
private:
	std::list<TextPiece*> pieces;

public:
	TextBoard();
	~TextBoard();

	void addPiece(TextPiece* piece);
	void draw();

	//setters/getters
	int getPieceCount() { return pieces.size(); }
};




