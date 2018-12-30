#pragma once
#include <iostream>
#include "Drawable.h"
#include "curses.h"
#include "ResourceManager.h"

class GameStateManager;
class GameState : public Drawable
{
protected:
	bool active = true;
	WINDOW* win;
	GameStateManager* manager;
	ResourceManager* resourceManager;
public:
	~GameState() { delwin(win);  } //not sure if this is being called
	virtual void processInput(GameStateManager& manager, int input) = 0;
	virtual void loadState() = 0;
	virtual void unloadState() = 0;
	virtual void initDefaults() {}

	//getters/setters
	void setWindow(WINDOW* win) { this->win = win; }
	bool isActive() { return active; }
	void setManager(GameStateManager* manager) { this->manager = manager; }
	GameStateManager* getManager() { return manager; }
	void setResourceManager(ResourceManager* rm) { resourceManager = rm; }
};
