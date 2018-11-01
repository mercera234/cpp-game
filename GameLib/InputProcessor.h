#pragma once
#include "ExitCode.h"

class InputProcessor
{
protected:
	int input;
	ExitCode exitCode = ExitCode::HANDLED;
public:
	virtual void processInput() = 0;


	//setters/getters

	void setInput(int inputIn) { input = inputIn; }
	int getInput() { return input; }

	ExitCode getExitCode() { return exitCode; }

};

