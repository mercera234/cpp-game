#pragma once
#include "Controllable.h"
#include <list>
using namespace std;

#define KEY_LISTENER 0x0001
#define MOUSE_LISTENER 0x0002

struct Registration
{
	Controllable* c;
	void(*callback) (void*, int);
	char listen_map = 0; //bit map of all listeners
	Registration() {};
};

struct KeyAccelerator
{
	int key;
	void(*callback) (void*, int);
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

public:

	void registerControl(Controllable* c, char listeners, void(*callback) (void*, int));
	void registerShortcutKey(int key, void(*callback) (void*, int));
	void popControl();
	//void registerMouseListener(Controllable* c, void (*callback) (void*, int));
	//void registerKeyListener(Controllable* c, void(*callback) (void*, int));

	Controllable* getFocus();
	void setFocus(Controllable* c) { focus = c; }
	//void cycleFocus();
	void prepareForShutdown() { shutdown = true; }
	bool handleInput(int input);
	void draw();
};