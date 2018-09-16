#pragma once
#include "curses.h"
#include <assert.h>
#include <map>

const int MAX_KEY_CODES = (KEY_MAX + 1);

template <typename T>
class InputManager
{
private:

	std::map<int, T> inputs;

	//T inputs[MAX_KEY_CODES];
public:
	InputManager() {}
	void fill(T input);
	bool setInput(int key, T input);
	T& getInput(int key); //get action for key supplied
};

template <typename T>
void InputManager<T>::fill(T input)
{
	std::fill(std::begin(inputs), std::end(inputs), input);
}

template <typename T>
T& InputManager<T>::getInput(int key)
{
	assert(key >= 0 && key < MAX_KEY_CODES);
	
	return inputs[key];
}

template <typename T>
bool InputManager<T>::setInput(int key, T input)
{
	assert(key >= 0 && key < MAX_KEY_CODES);

	inputs[key] = input;
	return true;
}


