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

	DialogBuilder dialogBuilder;
	DialogWindow mainMenuDialog;
	DialogWindow playerMenuDialog;
	DialogWindow descDialog;
	DialogWindow bodyDialog;
	

	
	//ControlHandle currBrowser;
	//SimpleCommand<MainMenu> browserCmd;

	//ItemBrowser itemBrowser;
	
	/*
	MainMenuBrowser mainBrowser;
	
	EquipBrowser equipBrowser;
	StatusBrowser statusBrowser;
	MapBrowser mapBrowser;
	ConfigBrowser configBrowser;
	SkillBrowser skillBrowser;
	
	
	*/

	//TODO just organizing things here to see patterns
	//SimpleCommand<ItemBrowser> itemCmd;
	void init();
	
	SimpleCommand<MainMenu> mainMenuCmd;
	void processMainMenuInput();

	SimpleCommand<MainMenu> playerMenuCmd;
	void processPlayerMenuInput();
	
	void setupStatusFields();
	void setupStatusContent();
	TextBoard statusContent;
	TextParamCurrMaxValue* hpRow, *mpRow;
	TextParamValue<BoundInt>* strengthRow, *defenseRow, *intelRow, *willRow, *agilityRow, *expRow;

	SimpleCommand<MainMenu> configMenuCmd;
	void processConfigMenuInput();
	//void processBrowserInput();

	//requires resource manager to be set
	int selectedAlly = -1;
public:
	MainMenu();
	MainMenu(ResourceManager* resourceManagerIn);
	void addPlayerParty(std::vector<Actor>& allies);
	void draw();
	int processInput(int input);
	void setWindow(WINDOW* win);

	~MainMenu();

	void setResourceManager(ResourceManager* resourceManagerIn);

	//setters/getters
	 
};

