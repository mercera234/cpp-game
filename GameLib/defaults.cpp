#include "defaults.h"
#include "GameInput.h"
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
	inputManager.setInput(CTRL_E, GameInput::TOGGLE_ENCOUNTERS);
}

int getInput(InputManager& inputManager)
{
	int input = getch();
	return inputManager.getInput(input);
}

void setupDefaultDataKeys(ResourceManager& rm)
{
	rm.theData.clearData();

	BoundInt gold$(0, 9999999, 0);
	rm.theData.storeIntData(GOLD$, gold$);
	
	BoundInt enemiesKilled(0, 9999999);
	rm.theData.storeIntData(ENEMIES_KILLED, enemiesKilled);
	
	BoundInt battlesWon(0, 9999999);
	rm.theData.storeIntData(BATTLES_WON, battlesWon);
	
	BoundInt stepsTaken(0, 9999999);
	rm.theData.storeIntData(STEPS, stepsTaken);
}