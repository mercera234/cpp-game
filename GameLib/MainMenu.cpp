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
#include "AutoMap.h"
#include "EquipControl.h"

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

	autoMapCmd.setReceiver(this);
	autoMapCmd.setAction(&MainMenu::processAutoMapInput);

	equipCmd.setReceiver(this);
	equipCmd.setAction(&MainMenu::processEquipInput);

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
	largestRect.setDimensions(bottomFrameHeight, leftFrameWidth + rightFrameWidth, Pos(topFrameHeight - 1, 0));
	
	dialogBuilder.buildMainMenu(mainMenuDialog, mainMenuRect);
	dialogBuilder.buildPlayerMenu(playerMenuDialog, playerRect);
	dialogBuilder.buildDesc(descDialog, descRect);
	dialogBuilder.buildMainMenuBody(bodyDialog, largeRect);
}


void MainMenu::processInput()
{
	exitCode = ExitCode::HANDLED;
	cm.setExitCode(exitCode);
	::processInput(cm, input);
}


void MainMenu::processMainMenuInput()
{
	GameInput input = (GameInput)cm.getInput();
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
	case MainMenuOption::EQUIP:
	{
		state = new EquipState;
		cm.setFocusedControl(&playerMenuDialog);

		GridMenu* playerMenu = (GridMenu*)playerMenuDialog.getControl();
		playerMenu->setCurrentItem(0);

		MenuItem* item = playerMenu->getCurrentItem();
		currPlayer = resourceManager->playerParty[item->index];
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
		DialogWindow* configDialog = new DialogWindow();
		dialogBuilder.buildConfigMenu(*configDialog, largestRect);

		cm.registerControl(configDialog, KEY_LISTENER, &configMenuCmd);
		cm.setFocusedControl(configDialog);
	}
	break;
	case MainMenuOption::MAP:
	{
		DialogWindow* autoMapDialog = new DialogWindow();
		dialogBuilder.buildAutoMap(*autoMapDialog, largestRect);

		cm.registerControl(autoMapDialog, KEY_LISTENER, &autoMapCmd);
		cm.setFocusedControl(autoMapDialog);
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
	GameInput input = (GameInput)mm->cm.getInput();

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
		DialogWindow* invDialog = (DialogWindow*)mm->cm.getTaggedControl(invDialogName);
		ItemBrowser* itemBrowser = (ItemBrowser*)invDialog->getControl();

		GameItem* item = itemBrowser->getCurrentItem();

		Actor& a = mm->resourceManager->playerParty[selectedPlayer->index];
		if (a.ingestConsumable(item))
		{
			itemBrowser->decrementItem();

			if (itemBrowser->getCurrentItem() == nullptr)
			{
				playerMenu->setCurrentItem(NO_CUR_ITEM);
				mm->cm.setFocusedControl(invDialog);
			}
		}
	}
}

void MainMenu::StatusState::processPlayerMenuInput(MainMenu* mm)
{
	GameInput input = (GameInput)mm->cm.getInput();

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

void MainMenu::EquipState::processPlayerMenuInput(MainMenu* mm)
{
	GameInput input = (GameInput)mm->cm.getInput();

	GridMenu* playerMenu = (GridMenu*)mm->playerMenuDialog.getControl();
	if (input == GameInput::CANCEL_INPUT)
	{
		playerMenu->setCurrentItem(NO_CUR_ITEM);
		//mm->cm.popControl();
		mm->cm.setFocusedControl(&mm->mainMenuDialog);
		return;
	}

	MenuItem* item = menuDriver(input, playerMenu);
	
	if (item) //ok is pressed
	{
		//mm->currPlayer = mm->resourceManager->playerParty[item->index];
		//dialogbuilder build equip menu
		DialogWindow* equipDialog = new DialogWindow();

		mm->dialogBuilder.buildEquipDialog(*equipDialog, mm->largestRect, mm->resourceManager->playerParty[item->index]);

		mm->cm.registerControl(equipDialog, KEY_LISTENER, &mm->equipCmd);
		mm->cm.setFocusedControl(equipDialog);
	}

}

void MainMenu::processEquipInput()
{
	DialogWindow* dWin = (DialogWindow*)cm.getFocusedControl();
	EquipControl* eCon = (EquipControl*)dWin->getControl();
	::processInput(*eCon, input);

	if (eCon->getExitCode() == ExitCode::GO_BACK)
	{
		cm.popControl();

		//delete dialog windows
		TUI::winMgr.delWin(dWin->getControl()->getWindow());
		TUI::winMgr.delWin(dWin->getWindow());
		delete dWin;

		cm.setFocusedControl(&playerMenuDialog);
		return;
	}
}

void MainMenu::processConfigMenuInput()
{
	int input = cm.getInput();

	DialogWindow* configDialog = (DialogWindow*)cm.getFocusedControl();
	ConfigMenu* menu = (ConfigMenu*)configDialog->getControl();

	if (input == GameInput::CANCEL_INPUT && menu->getEditState() == false) //don't accept cancel if we're editing a configuration item
	{
		cm.popControl();

		//delete dialog windows
		TUI::winMgr.delWin(menu->getWindow());
		TUI::winMgr.delWin(configDialog->getWindow());
		delete configDialog;

		cm.setFocusedControl(&mainMenuDialog);
		return;
	}


	menu->processInput(input);
}

void MainMenu::processAutoMapInput()
{
	int input = cm.getInput();

	DialogWindow* autoMapDialog = (DialogWindow*)cm.getFocusedControl();
	AutoMap* autoMap = (AutoMap*)autoMapDialog->getControl();

	if (input == GameInput::CANCEL_INPUT)
	{
		cm.popControl();

		//delete dialog windows
		TUI::winMgr.delWin(autoMap->getWindow());
		TUI::winMgr.delWin(autoMapDialog->getWindow());
		delete autoMapDialog;

		cm.setFocusedControl(&mainMenuDialog);
		return;
	}

	if (isInputDirectional((GameInput)input))
	{
		::processInput(*autoMap, getCursesKeyFromInput((GameInput)input));
	}
}

void MainMenu::setItemDescription(DialogWindow* invDialog, DialogWindow* itemDescDialog)
{
	ItemBrowser* browser = (ItemBrowser*)invDialog->getControl();

	TextLabel* lbl = (TextLabel*)itemDescDialog->getControl();

	GameItem* selectedItem = browser->getCurrentItem();
	
	std::string descText = selectedItem == nullptr ? "" : selectedItem->description;
	lbl->setText(descText);
}


void MainMenu::processItemInput()
{
	GameInput input = (GameInput)cm.getInput();

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
		DialogWindow* invDialog = (DialogWindow*)cm.getFocusedControl();
		ItemBrowser* browser = (ItemBrowser*)invDialog->getControl();
		GameItem* selectedItem = browser->getCurrentItem();

		if (selectedItem != nullptr && selectedItem->type == GameItemType::USABLE)
		{
			GridMenu* menu = (GridMenu*)playerMenuDialog.getControl();
			menu->setCurrentItem(0);
			cm.setFocusedControl(&playerMenuDialog);
		}
		return;
	} break;
	}

	DialogWindow* invDialog = (DialogWindow*)cm.getFocusedControl();
	DialogWindow* itemDescDialog = (DialogWindow*)cm.getTaggedControl(itemDescName);

	ItemBrowser* browser = (ItemBrowser*)invDialog->getControl();
	::processInput(*browser, getCursesKeyFromInput(input));

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
