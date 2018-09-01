#include "TestCommand.h"

void TestCommand::execute()
{
	mvaddstr(3, 3, "Executed.");
}