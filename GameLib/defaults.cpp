#include "defaults.h"
#include <algorithm>
#include "GameInput.h"
#include "TUI.h"

void loadDataFiles(ResourceManager& rm)
{
	//configuration file
	std::ifstream configStream(configFile);
	rm.loadConfigurationFile(configStream);

	defaultGameInputs(rm.inputs);

	setupDefaultDataKeys(rm);

	//actors
	std::ifstream actorStream(actorFile);
	rm.loadActorsFromTextFile(actorStream);

	//map rooms
	FileDirectory dataDir(dataDirName);
	rm.loadGameMapsFromDir(dataDir);

	//megamaps
	std::ifstream mapStream(mapFile);
	rm.loadMapsFromTextFile(mapStream);
}



void defaultGameInputs(std::map<int, Input>& inputs)
{
	for each (std::pair<int,Input> p in inputs)
	{
		int key = p.first;
		std::string action = p.second.name;
		std::string actionU(action.length(), ' ');
		std::transform(action.begin(), action.end(), actionU.begin(), toupper);

		GameInput gameInput = GameInput::UNKNOWN;

		if (actionU.compare("OK") == 0)
			gameInput = GameInput::OK_INPUT;
		else if (actionU.compare("CANCEL") == 0)
			gameInput = GameInput::CANCEL_INPUT;
		else if (actionU.compare("DOWN") == 0)
			gameInput = GameInput::DOWN_INPUT;
		else if (actionU.compare("UP") == 0)
			gameInput = GameInput::UP_INPUT;
		else if (actionU.compare("OPENMENU") == 0)
			gameInput = GameInput::OPEN_MENU_INPUT;
		else if (actionU.compare("LEFT") == 0)
			gameInput = GameInput::LEFT_INPUT;
		else if (actionU.compare("RIGHT") == 0)
			gameInput = GameInput::RIGHT_INPUT;
		else if (actionU.compare("CYCLELEFT") == 0)
			gameInput = GameInput::CYCLE_LEFT_INPUT;
		else if (actionU.compare("CYCLERIGHT") == 0)
			gameInput = GameInput::CYCLE_RIGHT_INPUT;
		
		inputs[key].code = gameInput;
	}

	//add secret inputs
	inputs.insert(std::make_pair(CTRL_F, Input("", GameInput::FIGHT_TRIGGER, true)));
	inputs.insert(std::make_pair(KEY_ESC, Input("", GameInput::QUIT_INPUT, true)));
	inputs.insert(std::make_pair(CTRL_E, Input("", GameInput::TOGGLE_ENCOUNTERS, true)));
	
}

int getInput(ResourceManager& resourceManager)
{
	int input = getch();
	if (resourceManager.getUseRawInput())
		return input;

	auto inputs = &resourceManager.inputs;
	auto it = inputs->find(input);

	return (it != inputs->end()) ? it->second.code : GameInput::UNKNOWN;
}

void setupDefaultDataKeys(ResourceManager& rm)
{
	rm.theData.clearData();

	BoundInt gold$(0, 9999999, 0);
	rm.theData.storeIntData(GOLD$, gold$);
	
	BoundInt enemiesKilled(0, 9999999);
	rm.theData.storeIntData(ENEMIES_KILLED, enemiesKilled);
	
	BoundInt battlesWon(0, 9999999);
	rm.theData.storeIntData(BATTLES_WON, battlesWon);
	
	BoundInt stepsTaken(0, 9999999);
	rm.theData.storeIntData(STEPS, stepsTaken);
}