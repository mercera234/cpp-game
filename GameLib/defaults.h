#pragma once
#include <iostream>
#include "InputManager.h"

//game defaults
const std::string player1Name = "Hero";
const int startingMapId = 12;
const int startY = 0;
const int startX = 0;

const std::string actorFile = "data\\actors.txt";
const std::string dataDirName = "data";

void defaultGameInputs(InputManager& inputManager);
int getInput(InputManager& inputManager);