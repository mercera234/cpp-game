#pragma once
#include "curses.h"

const int MAX_KEY_CODES = (KEY_MAX + 1);
const int NO_INPUT = -1;

class InputManager
{
private:
	int inputs[MAX_KEY_CODES];
public:
	InputManager();
	bool setInput(int key, int input);
	int getInput(int key); //get action for key supplied
};


