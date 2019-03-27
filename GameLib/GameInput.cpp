#include "GameInput.h"
#include "TUI.h"
#include "AbstractMenu.h"

bool isInputDirectional(GameInput input)
{
	return input == GameInput::UP_INPUT ||
		input == GameInput::DOWN_INPUT ||
		input == GameInput::RIGHT_INPUT ||
		input == GameInput::LEFT_INPUT;
}

int getCursesKeyFromInput(GameInput input)
{
	int cursesKey = -1;
	switch (input)
	{
		//These should all be only one move at a time
	case GameInput::UP_INPUT: cursesKey = KEY_UP; break;
	case GameInput::DOWN_INPUT: cursesKey = KEY_DOWN; break;
	case GameInput::LEFT_INPUT: cursesKey = KEY_LEFT; break;
	case GameInput::RIGHT_INPUT: cursesKey = KEY_RIGHT; break;
	case GameInput::OK_INPUT: cursesKey = KEY_ENTER; break;
	}
	return cursesKey;
}

