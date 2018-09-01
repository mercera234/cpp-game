#pragma once

#include "GameState.h"
#include "curses.h"
#include "ControlManager.h"
#include "GridMenu.h"
#include "Frame.h"
#include "MainMenu.h"

class MainMenuState : public GameState
{
private:
	MainMenuState(); //private so class is Singleton
	static GameState* instance;
	
	MainMenu mm;
	
	/*ControlManager cm;

	int leftFrameWidth;
	int rightFrameWidth;
	int topFrameHeight;
	int bottomFrameHeight;


	Frame* mainFrame;
	GridMenu mainMenu;

	Frame* playerFrame;
	GridMenu playerMenu;

	Frame* bodyFrame;
	WINDOW* bodyWindow;
	void drawBodyWindow();
	
	void processMainMenuInput(Frame* f, int input);
	void processPlayerMenuInput(Frame* f, int input);

	void setupMainMenu();
	void setupPlayerMenu();*/
public:
	static GameState* getInstance(); //since static, cannot be virtual in super class
	void processInput(GameStateManager& manager, int input);
	void draw();
	void loadState();
	void unloadState() {}

	/*static void mainMenuCallback(void* caller, void* ptr, int input);
	static void playerMenuCallback(void* caller, void* ptr, int input);*/
};