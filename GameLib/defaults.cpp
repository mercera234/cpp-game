#include "GameInput.h"
#include "InputManager.h"
#include "TUI.h"

void defaultGameInputs(InputManager& inputManager)
{
	inputManager.setInput('c', GameInput::OK_INPUT);
	inputManager.setInput('x', GameInput::CANCEL_INPUT);
	inputManager.setInput('d', GameInput::OPEN_MENU_INPUT);

	inputManager.setInput('C', GameInput::OK_INPUT);
	inputManager.setInput('X', GameInput::CANCEL_INPUT);
	inputManager.setInput('D', GameInput::OPEN_MENU_INPUT);

	inputManager.setInput(KEY_DOWN, GameInput::DOWN_INPUT);
	inputManager.setInput(KEY_UP, GameInput::UP_INPUT);
	inputManager.setInput(KEY_LEFT, GameInput::LEFT_INPUT);
	inputManager.setInput(KEY_RIGHT, GameInput::RIGHT_INPUT);

	//set up inputs for debugging
	inputManager.setInput(CTRL_F, GameInput::FIGHT_TRIGGER);
	inputManager.setInput(KEY_ESC, GameInput::QUIT_INPUT);
}

int getInput(InputManager& inputManager)
{
	int input = getch();
	return inputManager.getInput(input);
}