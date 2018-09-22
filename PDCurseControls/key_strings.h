#pragma once
#include <iostream>

/*
Return string corresponding to input key.
If verbose is true, the most human readable version will be returned.
*/
std::string getStringFromKey(int key, bool verbose = true);

/**/
int getKeyFromString(const std::string& text);

//key strings
const std::string UP = "UP";
const std::string DOWN = "DOWN";
const std::string LEFT = "LEFT";
const std::string RIGHT = "RIGHT";
const std::string SPACE = "SPACE";
const std::string ENTER = "ENTER";
