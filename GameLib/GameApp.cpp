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
	//setup input manager and default keys
	defaultGameInputs(inputManager);
	
	//load resources
	resourceManager.loadNullResources();
	setupDefaultDataKeys(resourceManager);

	std::ifstream is(actorFile);
	resourceManager.loadActorsFromTextFile(is);

	FileDirectory dataDir(dataDirName);
	resourceManager.loadGameMapsFromDir(dataDir);

	tui.init();
	
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
	resize_term(screenHeight, screenWidth);

	while (playing)
	{
		//draw
		stateMngr.draw();
		doupdate();

		//input/process
		int input = getInput(inputManager);
		playing = stateMngr.processInput(input);
			
	}

	return true;
}
