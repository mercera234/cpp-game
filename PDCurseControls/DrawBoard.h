#pragma once
#include "Controllable.h"

template<typename drawableObj>
class DrawBoard : public Controllable
{
private:
	std::list<drawableObj*> pieces;

public:
	DrawBoard() {}
	~DrawBoard();

	void addPiece(drawableObj* piece);
	void draw();

	//setters/getters
	int getPieceCount() { return pieces.size(); }
};

template<typename drawableObj>
DrawBoard<drawableObj>::~DrawBoard()
{
	for each (drawableObj* piece in pieces)
	{
		delete piece;
	}
}

template<typename drawableObj>
void DrawBoard<drawableObj>::addPiece(drawableObj* piece)
{
	pieces.push_back(piece);
}

template<typename drawableObj>
void DrawBoard<drawableObj>::draw()
{
	wclear(win);

	if (isFocused() || getStandout())
		wattron(win, A_BOLD);

	for each (drawableObj* piece in pieces)
	{
		piece->draw(win);
	}
	wnoutrefresh(win);
}