#pragma once
#include "Controllable.h"
#include "Frame.h"
#include "GridMenu.h"
#include "ControlManager.h"
#include "SimpleControlCommand.h"
#include "Actor.h"
#include "TextBoard.h"
#include "ResourceManager.h"
#include "TextParamValue.h"

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
	SimpleControlCommand<MainMenu> mainMenuCmd;
	SimpleControlCommand<MainMenu> playerMenuCmd;

	ResourceManager* resourceManager;

	int leftFrameWidth;
	int rightFrameWidth;
	int topFrameHeight;
	int bottomFrameHeight;


	Frame mainFrame;
	GridMenu mainMenu;

	Frame playerFrame;
	GridMenu playerMenu;

	Frame bodyFrame;
	TextBoard bodyContent;
	
	TextBoard statusContent;
	TextParamCurrMaxValue* hpRow, *mpRow;
	TextParamValue<BoundInt>* strengthRow, *defenseRow, *intelRow, *willRow, *agilityRow, *expRow;

	TextParamValue<BoundInt>* gold, *steps, *enemiesKilled, *battlesWon;

	Frame descFrame;
	GridMenu descMenu;

	int processMainMenuInput(Controllable* c, int input);
	int processPlayerMenuInput(Controllable* c, int input);

	void setupMainMenu();
	void setupPlayerMenu();

	void setupBodyFields();
	void setupHubContent();
	void setupStatusFields();
	void setupStatusContent();
	

	std::vector<Actor*> allies;
	int selectedAlly = -1;
public:
	MainMenu();
	void addPlayerParty(std::vector<Actor*>& allies);
	void draw();
	int processInput(int input);
	void setWindow(WINDOW* win);

	~MainMenu();

	void setResourceManager(ResourceManager* resourceManagerIn);

	//setters/getters
	 
};

