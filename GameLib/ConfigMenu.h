#pragma once
#include "Controllable.h"
#include "GridMenu.h"
#include "ResourceManager.h"
#include "Input.h"
#include <map>
#include "ExitCode.h"
#include "ConfigMenuItem.h"
/*
Config item is selected using key mapped to OK
configuration of item is canceled by pressing Cancel
Any other key selected changes the key if it is valid
*/
class ConfigMenu : public Controllable
{
private:
	ResourceManager* resourceManager;
	GridMenu menu;
	bool editState = false;

	bool validateKey(int input);
	void setMenuItems(std::map<int, Input>& inputs);
	void toggleEditState();
public:
	ConfigMenu() {}
	ConfigMenu(ResourceManager* resourceManagerIn);

	void setWindow(WINDOW* win);

	ExitCode processInput(int input);
	
	//sets the resource manager and creates the menu using the non-secret game inputs
	void setResourceManager(ResourceManager* resourceManagerIn);

	//override
	void setFocus(bool focusIn);
	void draw();
	

	//setters/getters

	ResourceManager* getResourceManager() { return resourceManager; }

	bool getEditState() { return editState; }
};

