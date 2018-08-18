#include "TUI.h"
//#include "stateManager.h"
#include "GridMenu.h"
#include "InputManager.h"


class GameApp
{
private:
	TUI tui;
	//StateManager stateManager;
	InputManager inputManager;

	int getInput();
	
	//int centerY, centerX;
	//int y, x; //stub!! character position on map(replaced later by actual character object)

	//bool fightInSession;

	
public:
	GameApp();

	/*The method that runs the game*/
	bool run();

	//game states
	static void fight(void*, int);
	static void mainMenuState(void*, int);
//	static void config(void*, int);

	static MenuItem* menuDriver(int input, AbstractMenu* m);

//	void initMainMenu();
	/*MenuResponse* menuDriver(Menu* menu, int input);
	MenuResponse* titleMenuDriver(int input);
	MenuResponse* mainMenuDriver(int input);*/
	//~GameApp();
};