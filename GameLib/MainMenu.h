#pragma once
#include "Controllable.h"
#include "Frame.h"
#include "GridMenu.h"
#include "ControlManager.h"
#include "SimpleCommand.h"
#include "Actor.h"
#include "TextBoard.h"
#include "ResourceManager.h"
#include "TextParamValue.h"
#include "DialogWindow.h"
#include "ItemBrowser.h"
#include "ControlHandle.h"
#include "MainMenuOption.h"
#include "DialogBuilder.h"
#include "LineItem.h"

/*The main menu to be used in game for checking player status, using inventory items, etc...
Designed for a 23 x 51 window. */
class MainMenu : public Controllable
{
private:
	ControlManager cm;
	ResourceManager* resourceManager;

	int leftFrameWidth;
	int rightFrameWidth;
	int topFrameHeight;
	int bottomFrameHeight;

	//convenience rects for building panels/windows
	Rect mainMenuRect; //upper left panel for main menu
	Rect playerRect; //lower left panel for player display
	Rect descRect; //upper right panel for descriptive comments
	Rect largeRect; //lower right panel for large area display

	DialogBuilder dialogBuilder;
	DialogWindow mainMenuDialog;
	DialogWindow playerMenuDialog;
	DialogWindow descDialog;
	DialogWindow bodyDialog;

	
	Actor currPlayer; //a copy of the current player used for displaying info
	GameItem* selectedItem;

	void init();
	
	SimpleCommand<MainMenu> mainMenuCmd;
	void processMainMenuInput();
	void processMainMenuSelection(LineItem* selection);

	SimpleCommand<MainMenu> playerMenuCmd;
	void processPlayerMenuInput();
	
	SimpleCommand<MainMenu> configMenuCmd;
	void processConfigMenuInput();

	SimpleCommand<MainMenu> itemCmd;
	void processItemInput();
	
	//inner classes for state
	struct SelectionState
	{
		virtual void processPlayerMenuInput(MainMenu*) = 0;
	};

	class InventoryState : public SelectionState
	{

	private:

	public:
		void processPlayerMenuInput(MainMenu*);
	};

	class StatusState : public SelectionState
	{

	private:

	public:
		void processPlayerMenuInput(MainMenu*);
	};

	SelectionState* state = nullptr;

public:
	MainMenu();
	MainMenu(ResourceManager* resourceManagerIn);
	
	void draw();
	int processInput(int input);
	void setWindow(WINDOW* win);

	~MainMenu();

	void setResourceManager(ResourceManager* resourceManagerIn);

	//setters/getters
	
};

