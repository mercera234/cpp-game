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

/*TODO simplify this class before adding item, equip, etc...*/
MainMenu::MainMenu()
{
	init();
}

//TODO doesn't work yet
MainMenu::MainMenu(ResourceManager* resourceManagerIn)
{
	init();
	setResourceManager(resourceManagerIn);
}

void MainMenu::init()
{
	cm.setCaller(this); //pretty sure I don't need this <-

	//setup cmds
	mainMenuCmd.setReceiver(this);
	mainMenuCmd.setAction(&MainMenu::processMainMenuInput);

	playerMenuCmd.setReceiver(this);
	playerMenuCmd.setAction(&MainMenu::processPlayerMenuInput);

	configMenuCmd.setReceiver(this);
	configMenuCmd.setAction(&MainMenu::processConfigMenuInput);

	/*browserCmd.setReceiver(this);
	browserCmd.setAction(&MainMenu::processBrowserInput);*/

	setupStatusFields();



	//register controls
	cm.registerControl(&mainMenuDialog, KEY_LISTENER, &mainMenuCmd);
	cm.registerControl(&playerMenuDialog, KEY_LISTENER, &playerMenuCmd);
	cm.registerControl(&descDialog, 0, nullptr);
	cm.registerControl(&bodyDialog, 0, nullptr);
	//cm.registerControl(&currBrowser, KEY_LISTENER, &browserCmd);

	cm.setFocusedControl(&mainMenuDialog);
}

void MainMenu::setupStatusFields()
{
	//values are null, but will be setup later
	hpRow = new TextParamCurrMaxValue(new LineFormat(0, Justf::LEFT), HP, nullptr);
	mpRow = new TextParamCurrMaxValue(new LineFormat(1, Justf::LEFT), MP, nullptr, 4);
	strengthRow = new TextParamValue<BoundInt>(new LineFormat(2, Justf::LEFT), STRENGTH, nullptr);
	defenseRow = new TextParamValue<BoundInt>(new LineFormat(3, Justf::LEFT), DEFENSE, nullptr);
	intelRow = new TextParamValue<BoundInt>(new LineFormat(4, Justf::LEFT), INTELLIGENCE, nullptr);
	willRow = new TextParamValue<BoundInt>(new LineFormat(5, Justf::LEFT), WILL, nullptr);
	agilityRow = new TextParamValue<BoundInt>(new LineFormat(6, Justf::LEFT), AGILITY, nullptr);
	expRow = new TextParamValue<BoundInt>(new LineFormat(7, Justf::LEFT), EXP, nullptr);

	statusContent.addPiece(hpRow); //the bound int values will need to be setup later
	statusContent.addPiece(mpRow);
	statusContent.addPiece(strengthRow);
	statusContent.addPiece(defenseRow);
	statusContent.addPiece(intelRow);
	statusContent.addPiece(willRow);
	statusContent.addPiece(agilityRow);
	statusContent.addPiece(expRow);
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
	dialogBuilder.buildDesc(descDialog, descRect, *resourceManager);
	dialogBuilder.buildMainMenuBody(bodyDialog, mainMenuBodyRect, *resourceManager);
	
	statusContent.setWindow(newwin(bottomFrameHeight - 2, rightFrameWidth - 2, 6, leftFrameWidth + 1));
}


void MainMenu::addPlayerParty(std::vector<Actor>& allies)
{
	assert(allies.size() <= playerCapacity);

	GridMenu* playerMenu = (GridMenu*)playerMenuDialog.getControl();
	int row = 0;
	for each (Actor actor in allies)
	{
		LineItem* item = (LineItem*)playerMenu->getItem(row++, 0);

		item->name = actor.name;
	}
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


	MenuItem* item = menuDriver(input, (AbstractMenu*)mainMenuDialog.getControl()); //TODO class is broken
	

	if (item)
	{
		switch (((LineItem*)item)->getCrossRef())
		{
		case MainMenuOption::INVENTORY:
		{
			/*currBrowser.control = &itemBrowser;
			itemBrowser.setWindow(bodyFrame.getWindow());

			itemBrowser.setItems(resourceManager->inventory);

			cm.setFocusedControl(&currBrowser);*/
		}
			break;
		case MainMenuOption::STATUS:
		{
			cm.setFocusedControl(&playerMenuDialog);
			GridMenu* playerMenu = (GridMenu*)playerMenuDialog.getControl();

			playerMenu->setCurrentItem(0);
			//TODO bodyFrame.setControl(&statusContent);
			setupStatusContent();
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
}

//void MainMenu::processBrowserInput()
//{
//	int input = cm.getInput();
//	Browser* b = (Browser*)currBrowser.control;
//	b->setInput(input);
//	b->processInput();
//
//	if (b->getExitCode() == ExitCode::GO_BACK)
//	{
//		cm.setFocusedControl(&mainFrame);
//		cm.unRegisterControl(&currBrowser);
//	}
//}

void MainMenu::processPlayerMenuInput()
{
	//right now this only handles the status menu!!!
	int input = cm.getInput();

	GridMenu* playerMenu = (GridMenu*)playerMenuDialog.getControl();
	if (input == GameInput::CANCEL_INPUT)
	{
		playerMenu->setCurrentItem(NO_CUR_ITEM);
		cm.setFocusedControl(&mainMenuDialog);
		//TODO bodyFrame.setControl(&bodyContent);
		return;
	}


	MenuItem* item = menuDriver(input, playerMenu);

	setupStatusContent();
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

void MainMenu::setupStatusContent()
{
	GridMenu* playerMenu = (GridMenu*)playerMenuDialog.getControl();
	MenuItem* item = playerMenu->getCurrentItem();

	if (item && ((LineItem*)item)->name.compare("") != 0)
	{
		Actor& ally = resourceManager->playerParty[item->index];

		//Actor* ally = allies[item->index];
		hpRow->setValue(&ally.getStat(StatType::HP));
		mpRow->setValue(&ally.getStat(StatType::MP));
		strengthRow->setValue(&ally.getStat(StatType::STRENGTH));
		defenseRow->setValue(&ally.getStat(StatType::DEFENSE));
		intelRow->setValue(&ally.getStat(StatType::INTELLIGENCE));
		willRow->setValue(&ally.getStat(StatType::WILL));
		agilityRow->setValue(&ally.getStat(StatType::AGILITY));
		expRow->setValue(&ally.getStat(StatType::EXP));
	}
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
