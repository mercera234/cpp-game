#pragma once
#include "Drawable.h"

class GameStateManager;
class GameState : public Drawable
{
protected:
	GameStateManager* manager;
	bool active = true;
	//static GameState* instance;
public:
	//virtual GameState* getInstance() = 0;
	virtual void processInput(int input) = 0;
	void setGameStateManager(GameStateManager* manager) { this->manager = manager; }
	GameStateManager* getGameStateManager() { return manager; }

	bool isActive() { return active; }
};
