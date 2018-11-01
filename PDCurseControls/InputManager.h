#pragma once
#include "curses.h"
#include "Input.h"
#include <assert.h>
#include <map>

class InputManager
{
private:

	std::map<int, Input> inputs;

	/*True if input provided matches the actual key pressed.
	False if input is to be abstracted behind a layer of user specific inputs as in a game.*/
	bool useRawInput = false;
	
public:
	InputManager() {}	
	int loadConfigurationFile(std::ifstream& textFile);

	/*Get input provided by user using pdcurses getch method.*/
	int getInput(); 

	/*Get input provided by key.*/
	int getInput(int key);
	
	//setters/getters
	
	auto& getInputs() { return inputs; }
	void setUseRawInput(bool useRealInputIn) { useRawInput = useRealInputIn; }
	bool getUseRawInput() { return useRawInput; }
};



