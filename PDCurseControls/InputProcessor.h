#pragma once
#include "ExitCode.h"

/*Interface for any object that handles input*/
class InputProcessor
{
protected:
	int input;
	ExitCode exitCode = ExitCode::HANDLED;
public:
	/*Process input that is provided using the setInput method.
	Return value is stored in exitCode.*/
	virtual void processInput() = 0;


	//setters/getters

	void setInput(int inputIn) { input = inputIn; }
	int getInput() { return input; }

	ExitCode getExitCode() { return exitCode; }

};

/*A standalone function that enforces setting input prior to it being processed and retrieving the exit code afterwards.*/
ExitCode processInput(InputProcessor& processor, int input);
