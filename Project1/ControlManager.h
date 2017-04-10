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

	//this should maybe be replaced with an iterator and prev/next pointers removed from Registration!
	list<Registration*>::iterator focus;

	bool active = true;
	void* caller; //the type of class that utilizes the Control Manager
	short cycleKey; //to cycle forward
	short revCycleKey; //to cycle backwards
	bool handleGlobalInput(int input);
	void handleControlInput(int input);
public:

	void registerControl(Controllable* c, char listeners, void(*callback) (void*, void*, int));
	//void registerControl(Controllable* c, char listeners, Command* cmd);
	void registerShortcutKey(int key, void(*callback) (void*, void*, int));
	void popControl();
	
	ControlManager(void* caller);// { this->caller = caller; }
	Controllable* getFocus();
	void setFocus(Controllable* c); 
	void cycleFocus(short cycleKey);
	void setActive(bool status) { active = status; }
	void shutdown();
	//void prepareForShutdown() {} //{ shutdown = true; }
	bool handleInput(int input);
	void draw();
};