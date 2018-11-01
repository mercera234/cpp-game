#pragma once
#include "Controllable.h"
#include "Direction.h"
#include "Style.h"
#include <list>
#include "TextPiece.h"

/*A control for displaying pieces of text in a space.*/
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




