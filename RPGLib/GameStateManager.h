#pragma once
#include <stack>
#include "GameState.h"
#include "Drawable.h"

class GameStateManager : public Drawable
{
private:
	GameState* state;
public:
	void setState(GameState* newState); 
	GameState* getState() { return state; }
	bool processInput(int input);
	void draw();
};
