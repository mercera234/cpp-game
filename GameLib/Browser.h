#pragma once
#include "DialogWindow.h"
#include "InputProcessor.h"
#include <list>
#include "TextBoard.h"
#include "SimpleCommand.h"
#include "ExitCode.h"
#include "ControlManager.h"

class Browser : public DialogWindow, public InputProcessor
{
protected:
	ControlManager cm;
	//SimpleCommand<Browser> cmd;
public:
	Browser() {}
	virtual void getDescription(std::list<std::string>& description) = 0;

	//setters/getters

	ControlManager& getControlManager() { return cm; }

};

template <class Receiver>
class Browser2 : public InputProcessor
{
protected:
	//ControlManager cm;
	SimpleCommand<Receiver> cmd;
	DialogWindow dialogWindow;
public:
	Browser2() {}
	
	void processInput();
	//setters/getters

	//ControlManager& getControlManager() { return cm; }

};