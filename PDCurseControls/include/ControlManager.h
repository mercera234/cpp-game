#pragma once
#include "Controllable.h"
#include <list>
#include <map>
#include <functional>
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


class ControlManager : public Drawable
{
private:

	/*
	Controls are in a stack order where the back is the top of the stack
	Input is tested starting at the back and continues to front until the appropriate control can handle the input
	*/
	std::list<Registration*> controls;
	//std::map<std::string, Registration, std::function<bool (const std::string&, const std::string&)>> controls2;
	//std::list<KeyAccelerator*> shortcuts;
	std::map<int, std::function<void(void*, void*, int)>> globalShortcuts; //shortcuts are global input

	//this should maybe be replaced with an iterator and prev/next pointers removed from Registration!
	std::list<Registration*>::iterator focusedControl;

	bool active = true;
	void* caller; //the type of class that utilizes the Control Manager
	short cycleKey; //to cycle forward
	short revCycleKey; //to cycle backwards
	bool handleGlobalInput(int input); //true if global input is handled
	bool handleControlInput(int input); //true if input is handled
	bool handleMouseInput(int input, Registration* r);
	bool handleKeyInput(int input, Registration* r);
	bool isGlobalInput(int input);

	bool authorizeCyclicKeyChoice(int key);
	void unsetFocus();
	void setFocusedControl(std::list<Registration*>::iterator it);
	void cycleFocus(short cycleKey);
public:
	ControlManager(void* caller);
	void registerControl(Controllable* c, char listeners, void(*callback) (void*, void*, int));
	//void registerControl(Controllable* c, char listeners, Command* cmd);
	void registerShortcutKey(int key, void(*callback) (void*, void*, int));
	void draw();
	/*
	Pop the control on top of the stack (end of the registered controls list).
	If the top element was the control with focus then focus shifts to the previous element in list.
	*/
	void popControl();
	
	/*
	Shuts down the control manager only if deactivated. Return true if shutdown is successful.
	This prevents a registered control from shutting down the manager thus resulting in the destruction of the calling control.
	*/
	bool shutdown();

	/*
	Handles input routed to control manager via key or mouse. 
	Returns true if successfully handled, false it nothing could handle it.
	*/
	bool handleInput(int input);

	//getters/setters

	/*
	Set the key used to cycle forwards through the registered controls.
	The user should be mindful not to use the same key for forward and reverse cycling.
	Returns true if key was authorized for cycling.
	*/
	bool setCycleKey(short key); 
	short getCycleKey() { return cycleKey; }

	/*
	Set the key used to cycle backwards through the registered controls.
	The user should be mindful not to use the same key for forward and reverse cycling.
	Returns true if key was authorized for cycling.
	*/
	bool setRevCycleKey(short key);
	short getRevCycleKey() { return revCycleKey; }

	/*
	Get the focused control. There will always only be one controllable with focus.
	*/
	Controllable* getFocus();

	/*
	Set c as the focused control. Assumes that c is already a registered control. If not then nothing happens.
	*/
	void setFocus(Controllable* c);
	
	int getControlCount() { return controls.size(); }
	int getKeyAcceleratorCount() { return globalShortcuts.size(); }
	void setActive(bool status) { active = status; }
	bool isActive() { return active; }
};