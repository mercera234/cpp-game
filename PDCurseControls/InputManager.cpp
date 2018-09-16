#include <algorithm>
#include <iterator>
#include "InputManager.h"

//InputManager::InputManager() 
//{
//	std::fill(std::begin(inputs), std::end(inputs), NO_INPUT);
//}
//
//int InputManager::getInput(int key)
//{
//	if (key < 0 || key >= MAX_KEY_CODES)
//		return NO_INPUT;
//
//	return inputs[key];
//}
//
//bool InputManager::setInput(int key, int input)
//{
//	if (key < 0 || key >= MAX_KEY_CODES)
//		return false;
//
//	inputs[key] = input;
//	return true;
//}