#pragma once
#include "InputProcessor.h"

class MockInputProcessor : public InputProcessor
{
private:
	int x = 5;
public:
	MockInputProcessor() {}
	void processInput();


	int getX() { return x; }

};

