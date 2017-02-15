#pragma once
#include "Controllable.h"
#include <list>
using namespace std;

#define KEY_LISTENER 0x0001
#define MOUSE_LISTENER 0x0002

struct Registration
{
	Controllable* c;
	void(*callback) (void*, void*, int);
	char listen_map = 0; //bit map of all listeners
	Registration() {};
};

struct KeyAccelerator
{
	int key;
	void(*callback) (void*, void*, int);
};

class ControlManager
{
private:

	//list<Controllable*> controls;
	list<Registration*> controls;
	list<KeyAccelerator*> shortcuts;
	//<Registration*> mouseList;
	//list<Registration*> keyList;
	Controllable* focus; //the item with focus
	bool shutdown = false; //if true then marks the CM for shutting down
	void* caller; //the type of class that utilizes the Control Manager

public:

	void registerControl(Controllable* c, char listeners, void(*callback) (void*, void*, int));
	void registerShortcutKey(int key, void(*callback) (void*, void*, int));
	void popControl();
	
	ControlManager(void* caller) { this->caller = caller; }
	Controllable* getFocus();
	void setFocus(Controllable* c) { focus = c; }
	//void cycleFocus();
	void prepareForShutdown() { shutdown = true; }
	bool handleInput(int input);
	bool handleGlobalInput(int input);
	void draw();
};