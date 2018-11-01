#include "GameApp.h"
#include "curses.h"
#include "LineItem.h"
#include <ctime>
#include "GameStateManager.h"
#include "TitleScreenState.h"
#include "ExploreState.h"
#include "GameInput.h"
#include "defaults.h"

GameApp::GameApp()
{	
	//load resources
	loadDataFiles(resourceManager);
	
	GameState* openingState = TitleScreenState::getInstance();
	openingState->setResourceManager(&resourceManager);

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
	TUI::centerConsoleWindow(gameScreenHeight, gameScreenWidth);

	while (playing)
	{
		//draw
		stateMngr.draw();
		doupdate();

		//input/process
		int input = resourceManager.getInputManager().getInput();
		playing = stateMngr.processInput(input);
			
	}

	return true;
}
