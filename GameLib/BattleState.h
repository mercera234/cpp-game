#pragma once

#include "GameState.h"
#include "BattleProcessor.h"

class BattleState : public GameState
{
private:
	BattleState(); //private so class is Singleton
	static GameState* instance;
	
	BattleProcessor battleProcessor;
public:
	static GameState* getInstance(); //since static, cannot be virtual in super class
	void processInput(GameStateManager& manager, int input);
	void draw();
	void loadState() {}
	void unloadState() {}
};