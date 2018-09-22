#pragma once
#include "Controllable.h"
#include "GridMenu.h"
#include "ResourceManager.h"
#include "Input.h"
#include <map>
#include "ExitCode.h"
/*
Config item is selected using key mapped to OK
configuration of item is canceled by pressing Cancel
Any other key selected changes the key if it is valid
*/
class ConfigMenu : public Controllable
{
private:
	bool validateKey(int input);
	ResourceManager* resourceManager;
	GridMenu menu;
public:
	bool editState = false;
	
	ConfigMenu();
	ConfigMenu(ResourceManager* resourceManagerIn);
	
	WINDOW* menuWin = nullptr;
	WINDOW* sideWin = nullptr;

	~ConfigMenu()
	{
		if(sideWin != nullptr)
			delwin(sideWin);
	}

	void setWindow(WINDOW* win);

	ExitCode processInput(int input);
	
	//sets the resource manager and creates the menu using the non-secret game inputs
	void setResourceManager(ResourceManager* resourceManagerIn);

	void draw();
	


	//setters/getters

	ResourceManager* getResourceManager() { return resourceManager; }


};

