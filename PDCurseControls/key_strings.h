#pragma once
#include <iostream>

/*
Return string corresponding to input key.
If verbose is true, the most human readable version will be returned.
*/
std::string getStringFromKey(int key, bool verbose = true);

/**/
int getKeyFromString(const std::string& text);