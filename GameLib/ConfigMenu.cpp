#include <string>
#include <algorithm>
#include "ConfigMenu.h"
#include "GameInput.h"
#include "ConfigMenuItem.h"
#include "menu_drivers.h"

ConfigMenu::ConfigMenu()
{
}


ConfigMenu::ConfigMenu(ResourceManager* resourceManagerIn)
{
	setResourceManager(resourceManagerIn);
}

void ConfigMenu::setResourceManager(ResourceManager* resourceManagerIn)
{
	resourceManager = resourceManagerIn;

	//this assumes that the inputs have already been setup prior to calling this method
	auto inputs = &resourceManager->getInputManager().getInputs();

	int nonSecretInputCount = std::count_if(inputs->begin(), inputs->end(),
		[](std::pair<int, Input> p)
		{
			return p.second.secret == false; //count all non-secret configurations
		}
	);

	menu.resetItems(nonSecretInputCount, 1); 

	int menuRow = 0;
	for (auto it = inputs->begin(); it != inputs->end(); it++)
	{
		int key = it->first;
		Input& input = it->second;

		if (input.secret)
			continue;

		
		menu.setItem(new ConfigMenuItem(key, input), menuRow++, 0);
	}

	menu.setCurrentItem(0);
	menu.post(true);
}

void ConfigMenu::setWindow(WINDOW* win)
{
	Controllable::setWindow(win);
	menuWin = newwin(getmaxy(win), getmaxx(win), getbegy(win), getbegx(win));
	menu.setWindow(menuWin);
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
	InputManager& inputMgr = resourceManager->getInputManager();
	if (editState)
	{
		assert(inputMgr.getUseRawInput());

		auto inputs = inputMgr.getInputs();

		if (inputs.count(input) == 1) //can't use a key that is already in use
			return HANDLED;

		if(validateKey(input) == false)
			return HANDLED;
		
		ConfigMenuItem* item = (ConfigMenuItem*)menu.getCurrentItem();
		
		//change input code in inputs
		int oldKey = item->getKey();
		Input& theInput = inputs[oldKey];
		inputs.insert(std::make_pair(input, theInput) );

		inputs.erase(oldKey);

		item->setKey(input); //should be validated input

		item->setEditing(false);
		editState = false;
		inputMgr.setUseRawInput(false);
	}
	else
	{
		MenuItem* item = menuDriver(input, &menu);

		if (item)
		{
			editState = true;
			((ConfigMenuItem*)item)->setEditing(true);
			inputMgr.setUseRawInput(true);
		}
	}

	return HANDLED;
}


void ConfigMenu::draw()
{
	menu.draw();
}