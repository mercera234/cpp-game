#pragma once
#include <string>
#include "Drawable.h"

class GameStateManager;
class GameState : public Drawable
{
protected:
	bool active = true;
	std::string name;
	void setName(const std::string& nameIn) { this->name = nameIn; } //only derived states should be allowed to use this method
public:
	virtual void processInput(GameStateManager& manager, int input) = 0;
	bool isActive() { return active; }

	//getters/setters
	std::string& getName() { return name; }
};
