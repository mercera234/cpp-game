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

//Main Menu options
enum MainMenuOption
{
	INVENTORY,
	EQUIP,
	STATUS,
	SKILL,
	CONFIG,
	MAP,
	SAVE,
	MAIN_QUIT
};

const int playerCapacity = 4;

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

	ControlHandle currBrowser;
	SimpleCommand<MainMenu> browserCmd;

	ItemBrowser itemBrowser;
	
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
	
	
	SimpleCommand<MainMenu> mainMenuCmd;
	void setupMainMenu();
	void processMainMenuInput();
	Frame mainFrame;
	GridMenu mainMenu;
	

	SimpleCommand<MainMenu> playerMenuCmd;
	void setupPlayerMenu();
	void processPlayerMenuInput();
	Frame playerFrame;
	GridMenu playerMenu;
	

	Frame bodyFrame;
	void setupBodyFields();
	TextBoard bodyContent;
	TextParamValue<BoundInt>* gold, *steps, *enemiesKilled, *battlesWon;

	
	void setupDescFields();
	Frame descFrame;
	TextBoard descContent;
	TextPiece* mapText;
	TextPiece* roomText;
	TextParamValue<std::string>* floor;

	
	void setupStatusFields();
	void setupStatusContent();
	TextBoard statusContent;
	TextParamCurrMaxValue* hpRow, *mpRow;
	TextParamValue<BoundInt>* strengthRow, *defenseRow, *intelRow, *willRow, *agilityRow, *expRow;

	SimpleCommand<MainMenu> configMenuCmd;
	void processConfigMenuInput();
	void processBrowserInput();

	//requires resource manager to be set
	void setupHubContent();
	void setupDescContent();

	int selectedAlly = -1;
public:
	MainMenu();
	void addPlayerParty(std::vector<Actor>& allies);
	void draw();
	int processInput(int input);
	void setWindow(WINDOW* win);

	~MainMenu();

	void setResourceManager(ResourceManager* resourceManagerIn);

	//setters/getters
	 
};

