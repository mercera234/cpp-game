#pragma once
#include <iostream>
#include "InputManager.h"
#include "ResourceManager.h"

void loadDataFiles(ResourceManager& rm);
void setupDefaultDataKeys(ResourceManager& rm);
void defaultGameInputs(std::map<int, Input>& inputManager);
//int getInput(std::map<int, Input>& inputs, bool useRawInput = false);
int getInput(ResourceManager& resourceManager);


//game defaults
const std::string player1Name = "Hero";
const int startingMapId = 5;
const int startY = 9;
const int startX = 3;

const int minPeaceSteps = 14;
const int maxPeaceSteps = 60;
const int encounterChance = 5;//percent chance

const std::string actorFile = "data\\actors.txt";
const std::string dataDirName = "data";
const std::string mapFile = "data\\maps.txt";
const std::string configFile = "data\\configuration.ini";

//////strings

//title menu items
const std::string GAME_TITLE = "---KILL, IMPROVE, REPEAT---";
const std::string S_NEW_GAME = "New Game";
const std::string S_LOAD_GAME = "Load Game";
const std::string S_QUIT_GAME = "Quit Game";


//main menu items
const std::string S_ITEM = "Item";
const std::string S_EQUIP = "Equip";
const std::string S_STATUS = "Status";
const std::string S_SKILL = "Skill";
const std::string S_MAP = "Map";
const std::string S_CONFIG = "Config";
const std::string S_SAVE = "Save";
const std::string S_QUIT = "Quit";

//Skill menu items
const std::string ATTACK = "Attack";
const std::string MAGIC = "Magic";
const std::string RUN = "Run";

//main menu stats
const std::string GOLD$ = "Gold$";
const std::string ENEMIES_KILLED = "Enemies Killed";
const std::string BATTLES_WON = "Battles Won";
const std::string STEPS = "Steps";

//actor stats
const std::string HP = "HP";
const std::string MP = "MP";
const std::string STRENGTH = "Strength";
const std::string DEFENSE = "Defense";
const std::string INTELLIGENCE = "Intel";
const std::string WILL = "Will";
const std::string AGILITY = "Agility";
const std::string EXP = "Exp";
const std::string TO_NEXT = "To Next";

