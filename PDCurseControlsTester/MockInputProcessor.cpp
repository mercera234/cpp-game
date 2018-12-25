#include "MockInputProcessor.h"


void MockInputProcessor::processInput()
{
	x += input;

	exitCode = x > 0 ? ExitCode::HANDLED : ExitCode::NOT_HANDLED;
}



