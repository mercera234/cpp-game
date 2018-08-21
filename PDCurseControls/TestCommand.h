#pragma once
#include "Command.h"
#include "curses.h"

class TestCommand : public Command
{
private:

public:
	void execute();

};


void TestCommand::execute()
{
	mvaddstr(3, 3, "Executed.");
}