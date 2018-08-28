#pragma once
#include <string>
#include "Drawable.h"
#include "curses.h"
#include "ResourceManager.h"

class GameStateManager;
class GameState : public Drawable
{
protected:
	bool active = true;
	std::string name;
	void setName(const std::string& nameIn) { this->name = nameIn; } //only derived states should be allowed to use this method
	WINDOW* win;
	GameStateManager* manager;
	ResourceManager* resourceManager;
public:
	
	~GameState() { delwin(win);  } //not sure if this is being called
	virtual void processInput(GameStateManager& manager, int input) = 0;
	bool isActive() { return active; }
	void setWindow(WINDOW* win) { this->win = win; }
	void setResourceManager(ResourceManager* rm) { resourceManager = rm; }
	virtual void loadState() = 0;
	virtual void unloadState() = 0;
	//getters/setters
	std::string& getName() { return name; }
	void setManager(GameStateManager* manager) { this->manager = manager; }
	GameStateManager* getManager() { return manager; }
};
