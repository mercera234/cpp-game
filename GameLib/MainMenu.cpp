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
#include "TextLabel.h"

const std::string itemDescName = "Item desc";
const std::string invDialogName = "Inv Dlg";

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

	mainMenuRect.setDimensions(topFrameHeight, leftFrameWidth, Pos(0, 0));
	playerRect.setDimensions(bottomFrameHeight, leftFrameWidth, Pos(topFrameHeight - 1, 0));
	descRect.setDimensions(topFrameHeight, rightFrameWidth, Pos(0, leftFrameWidth));
	largeRect.setDimensions(bottomFrameHeight, rightFrameWidth, Pos(topFrameHeight - 1, leftFrameWidth));
	
	dialogBuilder.buildMainMenu(mainMenuDialog, mainMenuRect);
	dialogBuilder.buildPlayerMenu(playerMenuDialog, playerRect);
	dialogBuilder.buildDesc(descDialog, descRect);
	dialogBuilder.buildMainMenuBody(bodyDialog, largeRect);
}


void MainMenu::processInput()
{
	exitCode = ExitCode::HANDLED;
	cm.setExitCode(exitCode);
	cm.handleInput(input);
}


void MainMenu::processMainMenuInput()
{
	int input = cm.getInput();
	if (input == GameInput::CANCEL_INPUT)
	{		
		exitCode = ExitCode::GO_BACK;
		return;
	}


	MenuItem* item = menuDriver(input, (AbstractMenu*)mainMenuDialog.getControl()); 
	
	if (item)
	{
		processMainMenuSelection((LineItem*)item);
	}
}

void MainMenu::processMainMenuSelection(LineItem* selection)
{
	switch (selection->getCrossRef())
	{
	case MainMenuOption::INVENTORY:
	{
		state = new InventoryState;
		DialogWindow* invDialog = new DialogWindow();
		dialogBuilder.buildInventory(*invDialog, largeRect);

		DialogWindow* itemDescDialog = new DialogWindow();
		dialogBuilder.buildCenteredTextWin(*itemDescDialog, descRect);

		cm.registerControl(itemDescDialog, 0, nullptr);
		cm.addTag(itemDescName, itemDescDialog);

		cm.registerControl(invDialog, KEY_LISTENER, &itemCmd);
		cm.addTag(invDialogName, invDialog);
		cm.setFocusedControl(invDialog);

		setItemDescription(invDialog, itemDescDialog);
	}
	break;
	case MainMenuOption::STATUS:
	{
		state = new StatusState;
		cm.setFocusedControl(&playerMenuDialog);
		DialogWindow* statusDialog = new DialogWindow();
		dialogBuilder.buildMainMenuStatus(*statusDialog, largeRect, currPlayer);

		GridMenu* playerMenu = (GridMenu*)playerMenuDialog.getControl();
		playerMenu->setCurrentItem(0);

		MenuItem* item = playerMenu->getCurrentItem();
		currPlayer = resourceManager->playerParty[item->index];

		cm.registerControl(statusDialog, 0, nullptr);
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
		exitCode = ExitCode::QUIT_TO_TITLE;
		return;
	}
}

void MainMenu::setResourceManager(ResourceManager* resourceManagerIn)
{
	resourceManager = resourceManagerIn;
	dialogBuilder.setRm(resourceManager);
}

void MainMenu::processPlayerMenuInput()
{
	state->processPlayerMenuInput(this);
}

void MainMenu::InventoryState::processPlayerMenuInput(MainMenu* mm)
{
	int input = mm->cm.getInput();

	GridMenu* playerMenu = (GridMenu*)mm->playerMenuDialog.getControl();

	switch (input)
	{
	case GameInput::CANCEL_INPUT:
	{
		playerMenu->setCurrentItem(NO_CUR_ITEM);
		Controllable* c = mm->cm.getTaggedControl(invDialogName);
		mm->cm.setFocusedControl(c);
		return;
	}
		break;

	}

	MenuItem* selectedPlayer = menuDriver(input, playerMenu);
	
	if (selectedPlayer)
	{
		Actor& a = mm->resourceManager->playerParty[selectedPlayer->index];
		a.ingestConsumable(*mm->selectedItem);
	}
}

void MainMenu::StatusState::processPlayerMenuInput(MainMenu* mm)
{
	int input = mm->cm.getInput();

	GridMenu* playerMenu = (GridMenu*)mm->playerMenuDialog.getControl();
	if (input == GameInput::CANCEL_INPUT)
	{
		playerMenu->setCurrentItem(NO_CUR_ITEM);
		mm->cm.popControl();
		mm->cm.setFocusedControl(&mm->mainMenuDialog);
		return;
	}

	menuDriver(input, playerMenu);
	MenuItem* item = playerMenu->getCurrentItem();

	if (item->index >= (int)mm->resourceManager->playerParty.size()) //a little sloppy here, but it works
	{
		playerMenu->setCurrentItem(item->index - 1);
		return;
	}
	else
		mm->currPlayer = mm->resourceManager->playerParty[item->index];
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

void MainMenu::setItemDescription(DialogWindow* invDialog, DialogWindow* itemDescDialog)
{
	ItemBrowser* browser = (ItemBrowser*)invDialog->getControl();

	OwnedItemRecord* record = browser->getCurrentItem();

	selectedItem = record->getPossession();

	TextLabel* lbl = (TextLabel*)itemDescDialog->getControl();
	lbl->setText(selectedItem->item->description);
}


void MainMenu::processItemInput()
{
	int input = cm.getInput();

	switch (input)
	{
	case GameInput::CANCEL_INPUT:
	{
		cm.removeTag(invDialogName);
		Controllable* itemBrowser = cm.popControl();

		cm.removeTag(itemDescName);
		Controllable* itemDescDialog = cm.popControl();

		delete itemBrowser;
		delete itemDescDialog;
		cm.setFocusedControl(&mainMenuDialog);
		return;
	}
		break;
	case GameInput::OK_INPUT:
	{
		GridMenu* menu = (GridMenu*)playerMenuDialog.getControl();
		menu->setCurrentItem(0);
		cm.setFocusedControl(&playerMenuDialog);
		return;
	} break;
	}

	DialogWindow* invDialog = (DialogWindow*)cm.getFocusedControl();
	DialogWindow* itemDescDialog = (DialogWindow*)cm.getTaggedControl(itemDescName);

	ItemBrowser* browser = (ItemBrowser*)invDialog->getControl();
	browser->processInput(input);

	setItemDescription(invDialog, itemDescDialog);
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
