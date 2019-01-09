#include <string>
#include <algorithm>
#include "ConfigMenu.h"
#include "GameInput.h"
#include "menu_drivers.h"


ConfigMenu::ConfigMenu(ResourceManager* resourceManagerIn)
{
	setResourceManager(resourceManagerIn);
}

void ConfigMenu::setResourceManager(ResourceManager* resourceManagerIn)
{
	resourceManager = resourceManagerIn;

	//this assumes that the inputs have already been setup prior to calling this method
	auto inputs = resourceManager->getInputManager().getInputs();

	int nonSecretInputCount = std::count_if(inputs.begin(), inputs.end(),
		[](std::pair<int, Input> p)
		{
			return p.second.secret == false; //count all non-secret configurations
		}
	);

	menu.resetItems(nonSecretInputCount, 1); 

	setMenuItems(inputs);

	menu.setCurrentItem(0);
	menu.post(true);
}

void ConfigMenu::setMenuItems(std::map<int, Input>& inputs)
{
	int menuRow = 0;
	for (auto it = inputs.begin(); it != inputs.end(); it++)
	{
		int key = it->first;
		Input& input = it->second;

		if (input.secret)
			continue;

		//set inputs in pre-determined order
		switch (input.code)
		{
		case GameInput::OK_INPUT: menuRow = 0; break;
		case GameInput::CANCEL_INPUT: menuRow = 1; break;
		case GameInput::OPEN_MENU_INPUT: menuRow = 2; break;
		case GameInput::CYCLE_LEFT_INPUT: menuRow = 3; break;
		case GameInput::CYCLE_RIGHT_INPUT: menuRow = 4; break;
		case GameInput::UP_INPUT: menuRow = 5; break;
		case GameInput::DOWN_INPUT: menuRow = 6; break;
		case GameInput::LEFT_INPUT: menuRow = 7; break;
		case GameInput::RIGHT_INPUT: menuRow = 8; break;
		}

		menu.setItem(new ConfigMenuItem(key, input), menuRow, 0);
	}
}

void ConfigMenu::setWindow(WINDOW* win)
{
	Controllable::setWindow(win);
	menu.setWindow(win);
}

bool ConfigMenu::validateKey(int input)
{
	if (input < '!') //input is less than lowest printable character
	{
		switch (input)
		{

		case '\n':
		case '\r':
		case ' ':
			return true;

			break;
		}
		
	}
	else if(input >= '!' && input <= KEY_DEL)
	{
		return true;
	}
	else
	{
		switch (input)
		{
		case KEY_DOWN:
		case KEY_UP:
		case KEY_LEFT:
		case KEY_RIGHT:
			return true;
		}
	}

	return false;
}


ExitCode ConfigMenu::processInput(int input)
{
	if (editState)
	{
		assert(resourceManager->getInputManager().getUseRawInput());

		auto& inputs = resourceManager->getInputManager().getInputs();

		if (inputs.count(input) == 1) //can't use a key that is already in use
		{
			//check if cancel key was pressed
			if (inputs[input].code == GameInput::CANCEL_INPUT)
			{
				toggleEditState();
			}
			return HANDLED;
		}
			
		if(validateKey(input) == false)
			return HANDLED;
		
		ConfigMenuItem* item = (ConfigMenuItem*)menu.getCurrentItem();
		
		//change input code in inputs
		int oldKey = item->getKey();
		Input& theInput = inputs[oldKey];
		inputs.insert(std::make_pair(input, theInput) );

		inputs.erase(oldKey);

		item->setKey(input); //should be validated input

		toggleEditState();
	}
	else
	{
		ConfigMenuItem* item = (ConfigMenuItem*)menuDriver(input, &menu);

		if (item)
		{
			toggleEditState();
		}
	}

	return HANDLED;
}

void ConfigMenu::toggleEditState()
{
	editState = !editState;
	ConfigMenuItem* item = (ConfigMenuItem*)menu.getCurrentItem();
	item->setEditing(editState);
	resourceManager->getInputManager().setUseRawInput(editState);
}

void ConfigMenu::setFocus(bool focusIn)
{
	Controllable::setFocus(focusIn);
	menu.setFocus(focusIn);
}


void ConfigMenu::draw()
{
	menu.draw();
}