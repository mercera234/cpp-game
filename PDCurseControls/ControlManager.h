#pragma once
#include "Controllable.h"
#include <list>
#include <vector>
#include <map>
#include <functional>
#include "Command.h"
#include "ControlCommand.h"
#include "input_return_codes.h"

#define KEY_LISTENER 0x01
#define MOUSE_LISTENER 0x02

//input to ControlManager was not handled by any registered controls
//const int NOT_HANDLED = -1;
//
////input was handled and no code was returned. Values greater than 0 imply a return code
//const int HANDLED = 0;

struct Registration
{
	Controllable* c;
	ControlCommand* cmd = nullptr;
	//int(*callback) (void*, void*, int);
	char listen_map = 0; //bit map of all listeners
	Registration() {};
};


class ControlManager : public Drawable
{
private:

	/*
	Controls are in a stack order where the back is the top of the stack
	Input is tested starting at the back and continues to front until the appropriate control can handle the input
	An actual <stack> is not used because we want to have access to all elements at all times
	*/
	std::vector<Registration*> controls;

	//a quick reference map used to quickly find the registration associated with a controllable
	std::map<Controllable*, Registration*, std::function<bool(Controllable*, Controllable*)> > quickRef;
	std::map<int, ControlCommand*> globalShortcuts; //shortcuts are global input
	//std::map<int, std::function<int(void*, void*, int)>> globalShortcuts; //shortcuts are global input

	/*if a control is focused, then this will point to it*/
	Registration* focusedReg = nullptr;

	bool active = true; //the active state of the Control Manager
	void* caller; //the type of class that utilizes the Control Manager
	short cycleKey; //to cycle focus forward to the next control
	short revCycleKey; //to cycle focus backwards to the previous control

	void setDefaultCycleKeys();
	int handleGlobalInput(int input);
	int handleControlInput(int input); 
	int handleMouseInput(int input, Registration* r);
	int handleKeyInput(int input, Registration* r);
	bool isGlobalInput(int input);

	bool authorizeCyclicKeyChoice(int key);
	void unsetFocus();
	void cycleFocus(short cycleKey);
public:
	ControlManager();
	~ControlManager();
	ControlManager(void* caller);
	void setCaller(void* caller);
	
	//void registerControl(Controllable* c, char listeners, int(*callback) (void*, void*, int));
	void registerControl(Controllable* c, char listeners, ControlCommand* cmd);
	//void registerControl(Controllable* c, char listeners, int(*callback) (void*, void*, int), Command* cmd);

	void unRegisterControl(Controllable* c);
	
	/*Shortcuts can only be called by the client object.
	TODO should this use a different kind of command that doesn't require a control?*/
	void registerShortcutKey(int key, ControlCommand* cmd);
	//void registerShortcutKey(int key, int(*callback) (void*, void*, int));
	void draw();
	/*
	Pop the control on top of the stack (end of the registered controls list).
	If the top element was the control with focus then focus shifts to the previous element in list.
	*/
	void popControl();
	
	/*Move the control to the top of the control stack(back of the list)
	This ensures it will always be drawn last*/
	void moveControlToTop(Controllable* c);

	Controllable* getTopControl() { return controls.back()->c; }
	/*
	Shuts down the control manager only if deactivated. Return true if shutdown is successful.
	This prevents a registered control from shutting down the manager thus resulting in the destruction of the calling control.
	*/
	bool shutdown();

	/*
	Handles input routed to control manager via key or mouse. 
	Returns true if successfully handled, false it nothing could handle it.
	*/
	int handleInput(int input);

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