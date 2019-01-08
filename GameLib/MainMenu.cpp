#include <assert.h>
#include "MainMenu.h"
#include "LineItem.h"
#include "menu_drivers.h"
#include "GameInput.h"
#include "TextBoard.h"
#include "TextParamValue.h"
#include "defaults.h"
#include "DialogWindow.h"
#include "ConfigMenu.h"
#include "LineFormat.h"

MainMenu::MainMenu()
{
	init();
}

MainMenu::MainMenu(ResourceManager* resourceManagerIn)
{
	init();
	setResourceManager(resourceManagerIn);
}

void MainMenu::init()
{
	//setup cmds
	mainMenuCmd.setReceiver(this);
	mainMenuCmd.setAction(&MainMenu::processMainMenuInput);

	playerMenuCmd.setReceiver(this);
	playerMenuCmd.setAction(&MainMenu::processPlayerMenuInput);

	configMenuCmd.setReceiver(this);
	configMenuCmd.setAction(&MainMenu::processConfigMenuInput);

	itemCmd.setReceiver(this);
	itemCmd.setAction(&MainMenu::processItemInput);

	//register controls
	cm.registerControl(&mainMenuDialog, KEY_LISTENER, &mainMenuCmd);
	cm.registerControl(&playerMenuDialog, KEY_LISTENER, &playerMenuCmd);
	cm.registerControl(&descDialog, 0, nullptr);
	cm.registerControl(&bodyDialog, 0, nullptr);
	
	cm.setFocusedControl(&mainMenuDialog);
}


void MainMenu::setWindow(WINDOW* win)
{
	Controllable::setWindow(win);

	int controlHeight = getmaxy(win);
	int controlWidth = getmaxx(win);

	topFrameHeight = 6;
	bottomFrameHeight = controlHeight - topFrameHeight + 1;
	leftFrameWidth = 20;
	rightFrameWidth = controlWidth - leftFrameWidth;

	Rect mainMenuRect(topFrameHeight, leftFrameWidth, Pos(0, 0));
	Rect playerMenuRect(bottomFrameHeight, leftFrameWidth, Pos(topFrameHeight - 1, 0));
	Rect descRect(topFrameHeight, rightFrameWidth, Pos(0, leftFrameWidth));
	Rect mainMenuBodyRect(bottomFrameHeight, rightFrameWidth, Pos(topFrameHeight - 1, leftFrameWidth));
	
	dialogBuilder.buildMainMenu(mainMenuDialog, mainMenuRect);
	dialogBuilder.buildPlayerMenu(playerMenuDialog, playerMenuRect);
	dialogBuilder.buildDesc(descDialog, descRect);
	dialogBuilder.buildMainMenuBody(bodyDialog, mainMenuBodyRect);

	//unseen dialog windows
	dialogBuilder.buildMainMenuStatus(statusDialog, mainMenuBodyRect, currPlayer);
}


int MainMenu::processInput(int input)
{
	cm.setExitCode(HANDLED);
	cm.handleInput(input);
	return cm.getExitCode();
}


void MainMenu::processMainMenuInput()
{
	int input = cm.getInput();
	if (input == GameInput::CANCEL_INPUT)
	{		
		cm.setExitCode(ExitCode::GO_BACK);
		return;
	}


	MenuItem* item = menuDriver(input, (AbstractMenu*)mainMenuDialog.getControl()); 
	

	if (item)
	{
		switch (((LineItem*)item)->getCrossRef())
		{
		case MainMenuOption::INVENTORY:
		{
			Rect itemRect(bottomFrameHeight, rightFrameWidth, Pos(topFrameHeight - 1, leftFrameWidth));
			dialogBuilder.buildInventory(invDialog, itemRect);

			cm.unRegisterControl(&bodyDialog);
			cm.registerControl(&invDialog, KEY_LISTENER, &itemCmd);
			cm.setFocusedControl(&invDialog);
		}
			break;
		case MainMenuOption::STATUS:
		{
			cm.setFocusedControl(&playerMenuDialog);
			
			GridMenu* playerMenu = (GridMenu*)playerMenuDialog.getControl();
			playerMenu->setCurrentItem(0);
			
			MenuItem* item = playerMenu->getCurrentItem();
			currPlayer = resourceManager->playerParty[item->index];

			cm.registerControl(&statusDialog, 0, nullptr);
			cm.unRegisterControl(&bodyDialog);
		}			
			break;
		case MainMenuOption::CONFIG:
		{
			ConfigMenu* configMenu = new ConfigMenu(resourceManager);
			DialogWindow* configWindow = new DialogWindow();

			int controlWidth = getmaxx(win);

			configWindow->setWindow(newwin(bottomFrameHeight, controlWidth, topFrameHeight - 1, 0));
			configWindow->setControl(configMenu);

			cm.registerControl(configWindow, KEY_LISTENER, &configMenuCmd);
			cm.setFocusedControl(configWindow);
		}
			
			break;
		case MainMenuOption::MAIN_QUIT: 
			cm.setExitCode(ExitCode::QUIT_TO_TITLE);
			return;
		}
	}
	else //no item selected, render default data
	{
		
	}
}

void MainMenu::setResourceManager(ResourceManager* resourceManagerIn)
{
	resourceManager = resourceManagerIn;
	dialogBuilder.setRm(resourceManager);
}

void MainMenu::processPlayerMenuInput()
{
	int input = cm.getInput();

	GridMenu* playerMenu = (GridMenu*)playerMenuDialog.getControl();
	if (input == GameInput::CANCEL_INPUT)
	{
		playerMenu->setCurrentItem(NO_CUR_ITEM);
		cm.setFocusedControl(&mainMenuDialog);
		cm.unRegisterControl(&statusDialog);
		cm.registerControl(&bodyDialog, 0, nullptr);
		return;
	}


	menuDriver(input, playerMenu);
	MenuItem* item = playerMenu->getCurrentItem();

	if (item->index >= (int)resourceManager->playerParty.size()) //a little sloppy here, but it works
		playerMenu->setCurrentItem(item->index - 1);
	else
		currPlayer = resourceManager->playerParty[item->index];
}

void MainMenu::processConfigMenuInput()
{
	int input = cm.getInput();

	DialogWindow* configDialog = (DialogWindow*)cm.getFocusedControl();
	ConfigMenu* menu = (ConfigMenu*)configDialog->getControl();

	if (input == GameInput::CANCEL_INPUT && menu->getEditState() == false) //don't accept cancel if we're editing a configuration item
	{
		cm.popControl();
		cm.setFocusedControl(&mainMenuDialog);
		return;
	}


	menu->processInput(input);
}

void MainMenu::processItemInput()
{
	int input = cm.getInput();

	if (input == GameInput::CANCEL_INPUT) 
	{
		cm.unRegisterControl(&invDialog);
		cm.registerControl(&bodyDialog, 0, nullptr);
		cm.setFocusedControl(&mainMenuDialog);
		return;
	}

	ItemBrowser* browser = (ItemBrowser*)invDialog.getControl();

	browser->processInput(input);

	OwnedItemRecord* record = browser->getCurrentItem();

	std::string desc = record->getPossession()->item->description;
}

void MainMenu::draw()
{
	werase(win);
	wnoutrefresh(win);
	cm.draw();
}

MainMenu::~MainMenu()
{
}
