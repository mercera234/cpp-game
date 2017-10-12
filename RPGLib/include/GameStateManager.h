#pragma once
#include <stack>
#include "GameState.h"
#include "Drawable.h"

class GameStateManager : public Drawable
{
private:
	GameState* state;
public:
	void setState(GameState* state);// { this->state = state; }
	bool processInput(int input);
	void draw();
	/*bool isAltered();
	void setStateAltered(bool);*/
};
