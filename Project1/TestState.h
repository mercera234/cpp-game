#pragma once
#include "GameState.h"

class TestState : public GameState
{
private:
	TestState() {}; //private so class is Singleton
	static GameState* instance;
public:
	static GameState* getInstance(); //since static, cannot be virtual in super class
	void processInput(int input);
	void draw();
};