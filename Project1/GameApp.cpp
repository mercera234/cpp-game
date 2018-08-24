#include "GameApp.h"
#include "curses.h"
#include "LineItem.h"
#include <ctime>
#include "GameStateManager.h"
#include "TitleScreenState.h"
#include "ExploreState.h"
#include "GameInput.h"

const std::string actorFile = "data\\actors.txt";
const std::string dataDirName = "data";

GameApp::GameApp()
{	
	//setup input manager and default keys
	inputManager.setInput('c', GameInput::OK_INPUT);
	inputManager.setInput('x', GameInput::CANCEL_INPUT);
	inputManager.setInput('d', GameInput::OPEN_MENU_INPUT);

	inputManager.setInput('C', GameInput::OK_INPUT);
	inputManager.setInput('X', GameInput::CANCEL_INPUT);
	inputManager.setInput('D', GameInput::OPEN_MENU_INPUT);

	inputManager.setInput(KEY_DOWN, GameInput::DOWN_INPUT);
	inputManager.setInput(KEY_UP, GameInput::UP_INPUT);
	inputManager.setInput(KEY_LEFT, GameInput::LEFT_INPUT);
	inputManager.setInput(KEY_RIGHT, GameInput::RIGHT_INPUT);

	//set up inputs for debugging
	inputManager.setInput(CTRL_F, GameInput::FIGHT_TRIGGER);
	inputManager.setInput(KEY_ESC, GameInput::QUIT_INPUT);

	std::ifstream is(actorFile);
	resourceManager.loadActorsFromTextFile(is);

	FileDirectory dataDir(dataDirName);
	resourceManager.loadGameMapsFromDir(dataDir);

	tui.init();
	
	TitleScreenState::getInstance();
	((TitleScreenState*)TitleScreenState::getInstance())->setResourceManager(&resourceManager);

}

int GameApp::getInput()
{
	int input = getch();
	return inputManager.getInput(input);
}

MenuItem* GameApp::menuDriver(int input, AbstractMenu* m) //static
{
	MenuItem* item = nullptr;
	int retval = -1;
	switch (input)
	{
	case GameInput::DOWN_INPUT: retval = m->driver(REQ_DOWN_ITEM); break;
	case GameInput::UP_INPUT: retval = m->driver(REQ_UP_ITEM); break;
	case GameInput::LEFT_INPUT: retval = m->driver(REQ_LEFT_ITEM); break;
	case GameInput::RIGHT_INPUT: retval = m->driver(REQ_RIGHT_ITEM); break;
		/*case KEY_PPAGE: m->driver(REQ_SCR_DPAGE); break;
		case KEY_NPAGE: m->driver(REQ_SCR_UPAGE); break;*/
	case GameInput::OK_INPUT:
		item = m->getCurrentItem();
		break;
	default: break;
	}

	return item;
}

bool GameApp::run()
{
	//input event loop
	GameStateManager stateMngr;
	stateMngr.setState(TitleScreenState::getInstance());
	
	bool playing = true;

	/*resize the terminal once before first draw. 
	Having it resize in the loop below before every draw causes issues.
	Lots of black gets drawn where there shouldn't be any*/
	resize_term(screenHeight, screenWidth);

	while (playing)
	{
		//draw
		stateMngr.draw();
		doupdate();

		//input/process
		switch (int input = getInput())
		{
		default:
			playing = stateMngr.processInput(input);
			break;
		}
	}

	return true;
}
