#include "GameInput.h"
#include "TUI.h"

bool isInputDirectional(GameInput input)
{
	return input == GameInput::UP_INPUT ||
		input == GameInput::DOWN_INPUT ||
		input == GameInput::RIGHT_INPUT ||
		input == GameInput::LEFT_INPUT;
}

int getKeyFromInput(GameInput input)
{
	int dirKey = -1;
	switch (input)
	{
		//These should all be only one move at a time
	case GameInput::UP_INPUT: dirKey = KEY_UP; break;
	case GameInput::DOWN_INPUT: dirKey = KEY_DOWN; break;
	case GameInput::LEFT_INPUT: dirKey = KEY_LEFT; break;
	case GameInput::RIGHT_INPUT: dirKey = KEY_RIGHT; break;
	}
	return dirKey;
}