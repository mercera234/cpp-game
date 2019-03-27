#pragma once
#include "Controllable.h"
#include <list>
#include <vector>
#include <map>
#include <functional>
#include "Command.h"
#include "ControlCommand.h"
#include "InputProcessor.h"

const int KEY_LISTENER = 0x01;
const int MOUSE_LISTENER = 0x02;


struct Registration
{
	Controllable* c;
	Command* cmd = nullptr;
	char listen_map = 0; //bit map of all listeners
	Registration() {};
};


class ControlManager : public Controllable, public InputProcessor
{
protected:

	/*
	Controls are in a stack order where the back is the top of the stack
	An actual <stack> is not used because we want to have access to all elements at all times
	*/
	std::vector<Registration*> controls;

	//a quick reference map used to quickly find the registration associated with a controllable
	std::map<Controllable*, Registration*, std::function<bool(Controllable*, Controllable*)> > quickRef;
	std::map<int, Command*> globalShortcuts; //shortcuts are global input
	std::map<std::string, Controllable*> tags; //a mapping of string names to controls 
	

	/*if a control is focused, then this will point to it*/
	Registration* focusedReg = nullptr;

	void* caller; //the type of class that utilizes the Control Manager
	short cycleKey; //to cycle focus forward to the next control
	short revCycleKey; //to cycle focus backwards to the previous control

	
	void setDefaultCycleKeys();
	void handleGlobalInput();
	void handleControlInput();
	virtual void handleMouseInput(Registration* r);
	void handleKeyInput(Registration* r);
	bool isGlobalInput(int input);
	void processModalInput();
	void processNonModalInput();

	virtual Registration* findMouseInputRecipient();

	bool authorizeCyclicKeyChoice(int key);
	void unsetFocus();
	void cycleFocus(short cycleKey);
public:
	ControlManager();
	~ControlManager();
	ControlManager(void* caller);
	void setCaller(void* caller);
	
	void registerControl(Controllable* c, char listeners, Command* cmd);

	/*Removes control from ControlManager, but does not delete the control*/
	void unRegisterControl(Controllable* c);
	
	/*Shortcuts can only be called by the client object.*/
	void registerShortcutKey(int key, Command* cmd);

	void draw();
	/*
	Pop the control on top of the stack (end of the registered controls list).
	If the top element was the control with focus then focus shifts to the previous element in list.
	*/
	Controllable* popControl();
	
	/*Move the control to the top of the control stack(back of the list)
	This ensures it will always be drawn last*/
	void moveControlToTop(Controllable* c);

	Controllable* getTopControl() { return controls.back()->c; }

	/*Add tag to control in manager. Return value if if tag was successful*/
	bool addTag(const std::string& name, Controllable* c);
	bool removeTag(const std::string& name); //remove tag from control manager

	/*Get the control previously tagged with tagName*/
	Controllable* getTaggedControl(const std::string& tagName);

	/*process input routed to control manager via key or mouse. */
	void processInput();

	//getters/setters

	int getInput() { return input; }


	void setExitCode(ExitCode exitCodeIn) { exitCode = exitCodeIn; }
	

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
	Controllable* getFocusedControl();

	/*
	Set c as the focused control. Assumes that c is already a registered control. If not then nothing happens.
	*/
	void setFocusedControl(Controllable* c);
	
	/*Unregister c and register with recipient using cmd.*/
	void passControl(Controllable* c, ControlManager& recipient, Command* cmd);

	//setters/getters

	int getControlCount() { return controls.size(); }
	int getKeyAcceleratorCount() { return globalShortcuts.size(); }
};