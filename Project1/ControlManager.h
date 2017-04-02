#pragma once
#include "Controllable.h"
#include <list>
using namespace std;
#include "Command.h"

#define KEY_LISTENER 0x0001
#define MOUSE_LISTENER 0x0002

struct Registration
{
	Controllable* c;
	Command* cmd;
	void(*callback) (void*, void*, int);
	char listen_map = 0; //bit map of all listeners
	Registration* prev;
	Registration* next;
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

	list<Registration*> controls;
	list<KeyAccelerator*> shortcuts;
	//Controllable* focusC; //the item with focus
	Registration* focus;
	bool shutdown = false; //if true then marks the CM for shutting down
	void* caller; //the type of class that utilizes the Control Manager
	short cycleKey; //to cycle forward
	short revCycleKey; //to cycle backwards

public:

	void registerControl(Controllable* c, char listeners, void(*callback) (void*, void*, int));
	void registerControl(Controllable* c, char listeners, Command* cmd);
	void registerShortcutKey(int key, void(*callback) (void*, void*, int));
	void popControl();
	
	ControlManager(void* caller) { this->caller = caller; }
	Controllable* getFocus();
	void setFocus(Controllable* c); 
	void cycleFocus(short cycleKey);
	void prepareForShutdown() { shutdown = true; }
	bool handleInput(int input);
	bool handleGlobalInput(int input);
	void draw();
};