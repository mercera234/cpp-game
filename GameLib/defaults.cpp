#include "defaults.h"
#include <algorithm>
#include "GameInput.h"
#include "TUI.h"
//#include "data_loaders.h"


void fixSpriteSymbols(std::list<Sprite*>& sprites)
{
	for (Sprite* sprite : sprites)
	{
		//setup sprites properly based on type
		if (GameItem* item = dynamic_cast<GameItem*>(sprite->thing))
		{
			switch (item->type)
			{
			case GameItemType::MONEY: sprite->symbol = moneySymbol;  break;
			default: sprite->symbol = itemSymbol; break;
			}
			sprite->impassible = false;
		}
	}
}

void loadDataFiles(ResourceManager& rm)
{
	//configuration file
	std::ifstream configStream(configFile);
	
	InputManager& inputMgr = rm.getInputManager();
	
	inputMgr.loadConfigurationFile(configStream);
	setupDefaultGameInputs(inputMgr.getInputs());

	setupDefaultDataKeys(rm);

	GameData& data = rm.getData();
	data.loadDataFile("data\\jsontest.txt"); //loads all the game data from json file

	//fix up sprites in each room TODO not sure if this is the best way to do this
	for (auto it = data.getRooms().begin(); it != data.getRooms().end(); it++)
	{
		MapRoom& room = it->second;
		fixSpriteSymbols(room.sprites);
	}
}



void setupDefaultGameInputs(std::map<int, Input>& inputs)
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

//int getInput(ResourceManager& resourceManager)
//{
//	int input = getch();
//	if (resourceManager.getUseRawInput())
//		return input;
//
//	auto inputs = &resourceManager.getData().getInputs();
//	auto it = inputs->find(input);
//
//	return (it != inputs->end()) ? it->second.code : GameInput::UNKNOWN;
//}

void setupDefaultDataKeys(ResourceManager& rm)
{
	GameData& data = rm.getData();
	data.clearData();

	BoundInt gold$(0, 9999999, 0);
	data.storeIntData(GOLD$, gold$);
	
	BoundInt enemiesKilled(0, 9999999);
	data.storeIntData(ENEMIES_KILLED, enemiesKilled);
	
	BoundInt battlesWon(0, 9999999);
	data.storeIntData(BATTLES_WON, battlesWon);
	
	BoundInt stepsTaken(0, 9999999);
	data.storeIntData(STEPS, stepsTaken);
}