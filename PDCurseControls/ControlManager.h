#pragma once
#include "Controllable.h"
#include <list>
#include <vector>
#include <map>
#include <functional>
#include "Command.h"


#define KEY_LISTENER 0x01
#define MOUSE_LISTENER 0x02

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
	An actual <stack> is not used because we want to have access to all elements at all times
	*/
	std::vector<Registration*> controls;

	//a quick reference map used to quickly find the registration associated with a controllable
	std::map<Controllable*, Registration*, std::function<bool(Controllable*, Controllable*)> > quickRef;
	std::map<int, std::function<void(void*, void*, int)>> globalShortcuts; //shortcuts are global input

	/*if a control is focused, then this will point to it
	If no controls have focus, then it will still to point to one control*/
	//int focusNdx = -1;
	Registration* focusedReg = nullptr;
	//std::vector<Registration*>::iterator focusedControl;
	
	 

	bool active = true; //the active state of the Control Manager
	void* caller; //the type of class that utilizes the Control Manager
	short cycleKey; //to cycle focus forward to the next control
	short revCycleKey; //to cycle focus backwards to the previous control

	void setDefaultCycleKeys();
	bool handleGlobalInput(int input); //true if global input is handled
	bool handleControlInput(int input); //true if input is handled
	bool handleMouseInput(int input, Registration* r);
	bool handleKeyInput(int input, Registration* r);
	bool isGlobalInput(int input);

	bool authorizeCyclicKeyChoice(int key);
	void unsetFocus();
	void cycleFocus(short cycleKey);
public:
	ControlManager();
	~ControlManager();
	ControlManager(void* caller);
	void setCaller(void* caller);
	
	void registerControl(Controllable* c, char listeners, void(*callback) (void*, void*, int));
	void unRegisterControl(Controllable* c);
	//void registerControl(Controllable* c, char listeners, Command* cmd);
	void registerShortcutKey(int key, void(*callback) (void*, void*, int));
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