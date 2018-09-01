#pragma once

class Command
{
private:
public:
	virtual void execute() = 0;
	virtual int execute(int) = 0;
};

