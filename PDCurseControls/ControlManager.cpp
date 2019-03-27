#include "ControlManager.h"
#include "curses.h"
#include "compare_functions.h"
#include <iterator>
#include <algorithm>
#include <assert.h>

const short defaultCycleKey = '\t';
const short defaultRevCycleKey = KEY_BTAB;

ControlManager::ControlManager() : quickRef(comparePointers<Controllable>)
{
	setDefaultCycleKeys();
}

ControlManager::ControlManager(void* caller) : quickRef(comparePointers<Controllable>)
{ 
	setCaller(caller);

	//set default cycle keys
	setDefaultCycleKeys();
}

void ControlManager::setCaller(void* caller)
{
	this->caller = caller;
}

void ControlManager::setDefaultCycleKeys()
{
	setCycleKey(defaultCycleKey);
	setRevCycleKey(defaultRevCycleKey);
}

bool ControlManager::setCycleKey(short key)
{
	if (authorizeCyclicKeyChoice(key) == false)
		return false; //key is not set

	cycleKey = key;
	return true;
}

bool ControlManager::setRevCycleKey(short key)
{
	if (authorizeCyclicKeyChoice(key) == false)
		return false; //key is not set

	revCycleKey = key;
	return true;
}


bool ControlManager::authorizeCyclicKeyChoice(int key)
{
	//authorize key
	switch (key)
	{ //below is list of unauthorized keys
	case KEY_MOUSE:
		key = defaultCycleKey;
		return false;
	}

	return true;
}


void ControlManager::registerControl(Controllable* c, char listeners, Command* cmd)
{
	Registration* r = new Registration();

	r->c = c;
	
	r->cmd = cmd;
	r->listen_map = listeners;


	controls.push_back(r);
	quickRef.insert(std::make_pair(c, r));

	c->setFocus(false); //unfocus all newly registered controls by default

	assert(quickRef.size() == controls.size());
}


void ControlManager::unRegisterControl(Controllable* c)
{
	//remove focus
	auto it = quickRef.find(c);
	if (it == quickRef.end()) //control is not registered
		return;

	Registration* r = it->second;

	if(r == focusedReg)
		unsetFocus();

	//remove control
	auto it2 = std::find(controls.begin(), controls.end(), r);

	controls.erase(it2);
	quickRef.erase(c);

	assert(quickRef.size() == controls.size());
}

void ControlManager::registerShortcutKey(int key, Command* cmd)
{
	auto shortcutMapping = std::make_pair(key, cmd);
	globalShortcuts.insert(shortcutMapping);
}


Controllable* ControlManager::popControl()
{
	if(controls.back() == focusedReg) //if the control being popped is focused
	{
		//if object was focused then set focus to next 
		for (auto it = ++controls.rbegin(); it != controls.rend(); it++)//we start from the end, so top most window is processed first
		{
			Registration* r = *it;
			Controllable* c = r->c;

			if (c->isFocusable())
			{
				setFocusedControl(c);
				break;
			}
		}
	}

	Controllable* poppedControl = controls.back()->c;
	unRegisterControl(poppedControl);
	return poppedControl;
}

void ControlManager::unsetFocus()
{
	if(focusedReg == nullptr)
		return;

	focusedReg->c->setFocus(false);
}

Controllable* ControlManager::getFocusedControl()
{
	if (focusedReg == nullptr) //nothing to get
		return nullptr;

	return focusedReg->c;
}

void ControlManager::cycleFocus(short key)
{
	if (controls.empty())
		return;

	//unset focus on current controllable
	auto it = std::find(controls.begin(), controls.end(), focusedReg);
	unsetFocus();

	do //cycle to next control (next in vector) until we find one that is focusable
	{
		if (key == cycleKey)
		{
			it++;
			if (it == controls.end())
					it = controls.begin();
		}
		else if (key == revCycleKey) //cycle to previous control (prev in vector) until we find one that is focusable
		{
			if (it == controls.begin())
				it = --controls.end();
			else
				it--;
		}
	} while ( (*it)->c->isFocusable() == false);
	setFocusedControl((*it)->c);
}


void ControlManager::moveControlToTop(Controllable* c)
{
	//find the corresponding registration
	auto it = quickRef.find(c);
	if (it == quickRef.end()) //control is not registered
		return;

	auto beginIt = std::find(controls.begin(), controls.end(), it->second);

	int rangeSize = std::distance(beginIt, controls.end());
	int startNdx = std::distance(controls.begin(), beginIt);
	//range should include control to move up to end
	std::rotate(beginIt, beginIt + (rangeSize - startNdx), controls.end());
}


void ControlManager::setFocusedControl(Controllable* c)
{ 
	auto it = quickRef.find(c);
	if (it == quickRef.end())
		return;

	unsetFocus();
	
	Registration* r = it->second;
	focusedReg = r;
	r->c->setFocus(true);
} 

void ControlManager::handleGlobalInput()
{
	//check for cycle key
	if (input == cycleKey || input == revCycleKey)
	{
		cycleFocus(input);
		return;
	}

	auto shortcutMapping = globalShortcuts.find(input);
	auto mapping = *shortcutMapping;
	auto cmd = mapping.second; 

	cmd->execute();
}

bool ControlManager::isGlobalInput(int input)
{
	return input == cycleKey || input == revCycleKey || (globalShortcuts.count(input) > 0);
}

void ControlManager::processInput()
{
	if (controls.empty() == false) //there are controls
	{
		if (focusedReg->c->isModal())
			handleControlInput();
		else if (isGlobalInput(input))
			handleGlobalInput();
		else
			handleControlInput();
	}
	else //there are no controls, input can only be global
	{
		if (isGlobalInput(input))
			handleGlobalInput();
		else
			setExitCode(ExitCode::NOT_HANDLED);
	}
}


void ControlManager::handleControlInput()
{
	//check first if focused control is Modal. If so, then it is the only control capable of processing input	
	(focusedReg->c->isModal()) ? processModalInput() : processNonModalInput();
}

void ControlManager::processModalInput()
{
	if (focusedReg->cmd == nullptr) //no point in checking registration with no callback
	{
		setExitCode(ExitCode::NOT_HANDLED);
		return; //modal window cannot handle the input (this implies a soft lock so design your UI carefully!)
	}
		

	if (input == KEY_MOUSE && (focusedReg->listen_map & MOUSE_LISTENER))
		handleMouseInput(focusedReg);
	else if (input != KEY_MOUSE && focusedReg->listen_map & KEY_LISTENER)
		handleKeyInput(focusedReg);
}

void ControlManager::processNonModalInput()
{
	int exitCode = NOT_HANDLED;
	if (input == KEY_MOUSE) //input could be delivered to any control
	{
		//find control being operated on
		Registration* r = findMouseInputRecipient();
		if (r == nullptr)
		{
			setExitCode(ExitCode::NOT_HANDLED);
			return;
		}
			

		handleMouseInput(r);
	}
	else if (focusedReg->listen_map & KEY_LISTENER)//keyboard input goes directly to focused control only, which is capable of handling key input
	{
		handleKeyInput(focusedReg);
	}
}

Registration* ControlManager::findMouseInputRecipient()
{
	for (auto it = controls.rbegin(); it != controls.rend(); it++) //we start from the end so highest overlapping windows are processed first
	{
		Registration* r = *it;
		Controllable* c = r->c;
	/*	MEVENT event;
		nc_getmouse(&event);*/

		MEVENT* event = getMouse();

		//control contains the mouse click, and can respond to mouse events
		if (wenclose(c->getWindow(), event->y, event->x) && (r->listen_map & MOUSE_LISTENER))
		{
			//if (dynamic_cast<ControlManager*>(c)) //if control is a manager, then check all controls and back out if none found
			//{
			//	ControlManager* cm = (ControlManager*)c;
			//	cm->handleInput(input);
			//}
			
			return r;
		}
			
	}

	return nullptr;
}


void ControlManager::handleKeyInput(Registration* r)
{
	if (focusedReg != r)
	{
		setExitCode(ExitCode::NOT_HANDLED);
		return;
	}
		

	r->cmd->execute();
}

void ControlManager::handleMouseInput(Registration* r)
{
	Controllable* c = r->c;
	/*MEVENT event;
	nc_getmouse(&event);*/
	MEVENT* event = getMouse();

	if (wenclose(c->getWindow(), event->y, event->x))
	{
		Registration* prevFocusedReg = focusedReg;
		setFocusedControl(c);		
		r->cmd->execute();

		if (c->isFocusable() == false)
			setFocusedControl(prevFocusedReg->c);
	}
	else
		setExitCode(ExitCode::NOT_HANDLED);
}


void ControlManager::passControl(Controllable* c, ControlManager& recipient, Command* cmd)
{
	auto it = quickRef.find(c);
	if (it == quickRef.end()) //control is not registered
		return;

	Registration* r = it->second;
	char listenMap = r->listen_map;

	unRegisterControl(c); //if not registered, then nothing happens
	
	recipient.registerControl(c, listenMap, cmd);
}

bool ControlManager::addTag(const std::string& name, Controllable* c)
{
	assert(tags.count(name) == 0); //why would we ever attempt to add two tags with the same name?

	tags.insert(std::make_pair(name, c));
	return true;
}

bool ControlManager::removeTag(const std::string& name)
{
	return tags.erase(name) > 0;
}

/*Get the control previously tagged with tagName*/
Controllable* ControlManager::getTaggedControl(const std::string& tagName)
{
	if (tags.count(tagName) < 1)
		return nullptr;

	return tags[tagName];
}


void ControlManager::draw()
{
	//draw all layers from bottom to top
	for (auto it = controls.begin(); it != controls.end(); it++)
	{
		Registration* r = *it;
		Controllable* c = r->c;
		if(c->getShowing())
			c->draw();
	}
	if(focusedReg != nullptr)
		focusedReg->c->setFocus(true); //set focus again in case the cursor is involved
}

ControlManager::~ControlManager()
{
	for each (Registration* r in controls)
	{
		delete r;
	}
}



