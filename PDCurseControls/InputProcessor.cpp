#include "InputProcessor.h"

ExitCode processInput(InputProcessor& processor, int input)
{
	processor.setInput(input);
	processor.processInput();
	return processor.getExitCode();
}