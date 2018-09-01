#pragma once
#include "Controllable.h"
#include "Frame.h"
#include "GridMenu.h"
#include "ControlManager.h"
#include "SimpleControlCommand.h"
#include "Actor.h"

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

const int exitCode = 1;
const int quitCode = 2;

class MainMenu : public Controllable
{
private:
	ControlManager cm;
	SimpleControlCommand<MainMenu> mainMenuCmd;
	SimpleControlCommand<MainMenu> playerMenuCmd;


	int leftFrameWidth;
	int rightFrameWidth;
	int topFrameHeight;
	int bottomFrameHeight;




	Frame mainFrame;
	GridMenu mainMenu;

	Frame playerFrame;
	GridMenu playerMenu;

	Frame bodyFrame;
	WINDOW* bodyWindow;
	void drawBodyWindow();
	

	int processMainMenuInput(Controllable* c, int input);
	int processPlayerMenuInput(Controllable* c, int input);

	void setupMainMenu();
	void setupPlayerMenu();

	std::vector<Actor*> allies;
	int selectedAlly = -1;
public:
	MainMenu();
	void addPlayerParty(std::vector<Actor*>& allies);
	void draw();
	int processInput(int input);

	~MainMenu();

	static int mainMenuCallback(void* caller, void* ptr, int input);
	static int playerMenuCallback(void* caller, void* ptr, int input);
};

