#pragma once
#include "MenuItem.h"
#include "Input.h"

class ConfigMenuItem : public MenuItem
{
private:
	Input input;
	int key;
	bool editing = false;
	std::pair<int, Input>* inputPair;
public:
	ConfigMenuItem() {}
	ConfigMenuItem(int keyIn, Input& inputIn);
	
	~ConfigMenuItem();

	
	//setters/getters

	void setConfigInput(int keyIn, Input& inputIn) { key = keyIn;  input = inputIn; }
	Input& getInput() { return input; }

	int getKey() { return key; }
	void setKey(int keyIn) { key = keyIn; }

	void setEditing(bool editingIn) { editing = editingIn; }
	bool getEditing() { return editing; }


	void draw();
	

};

