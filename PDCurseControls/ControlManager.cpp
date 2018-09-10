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


void ControlManager::registerControl(Controllable* c, char listeners, ControlCommand* cmd)
{
	Registration* r = new Registration();

	r->c = c;
	c->setControlManager(this);
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
	auto it2 = std::find(controls.begin(), controls.end(), r);

	controls.erase(it2);

	//remove doesn't work for some reason!
	//auto it2 = std::remove(controls.begin(), controls.end(), r);

	quickRef.erase(c);

	assert(quickRef.size() == controls.size());
}

void ControlManager::registerShortcutKey(int key, ControlCommand* cmd)
{
	auto shortcutMapping = std::make_pair(key, cmd);
	globalShortcuts.insert(shortcutMapping);
}


void ControlManager::popControl()
{
	if(controls.back() == focusedReg) //if the control being popped is focused
	{
		focusedReg = nullptr;
		//if object was focused then set focus to next 
		for (auto it = ++controls.rbegin(); it != controls.rend(); it++)//we start from the end, so modal windows are always processed first
		{
			Registration* r = *it;
			Controllable* c = r->c;

			if (c->isFocusable())
			{
				setFocus(c);
				break;
			}
		}
	}

	unRegisterControl(controls.back()->c);
}

void ControlManager::unsetFocus()
{
	if(focusedReg == nullptr)
		return;

	focusedReg->c->setFocus(false);
}

Controllable* ControlManager::getFocus()
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
	setFocus((*it)->c);
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


void ControlManager::setFocus(Controllable* c)
{ 
	if (c->isFocusable() == false)
		return;

	unsetFocus();

	auto it = quickRef.find(c);

	Registration* r = it->second;
	focusedReg = r;
	r->c->setFocus(true);
} 

int ControlManager::handleGlobalInput(int input)
{
	//check for cycle key
	if (input == cycleKey || input == revCycleKey)
	{
		cycleFocus(input);
		return HANDLED;
	}

	auto shortcutMapping = globalShortcuts.find(input);
	auto mapping = *shortcutMapping;
	auto cmd = mapping.second; 

	return cmd->execute(nullptr, input);
}

bool ControlManager::isGlobalInput(int input)
{
	return input == cycleKey || input == revCycleKey || (globalShortcuts.count(input) > 0);
}

int ControlManager::handleInput(int input)
{
	int handled = NOT_HANDLED;
	if (controls.empty() == false) //there are controls
	{
		if (focusedReg->c->isModal())
			handled = handleControlInput(input);
		else if (isGlobalInput(input))
			handled = handleGlobalInput(input);
		else
			handled = handleControlInput(input);
	}
	else //there are no controls, input can only be global
	{
		if(isGlobalInput(input))
			handled = handleGlobalInput(input);
	}

	return handled;
}

int ControlManager::handleControlInput(int input)
{
	//check first if focused control is Modal. If so, then it is the only control capable of processing input	
	return (focusedReg->c->isModal()) ? processModalInput(input) : processNonModalInput(input);
}

int ControlManager::processModalInput(int input)
{
	int exitCode = NOT_HANDLED;

	if (focusedReg->cmd == nullptr) //no point in checking registration with no callback
		return exitCode; //modal window cannot handle the input (this implies a soft lock so design your UI carefully!)

	if (input == KEY_MOUSE && (focusedReg->listen_map & MOUSE_LISTENER))
		exitCode = handleMouseInput(input, focusedReg);
	else if (input != KEY_MOUSE && focusedReg->listen_map & KEY_LISTENER)
		exitCode = handleKeyInput(input, focusedReg);

	return exitCode;
}

int ControlManager::processNonModalInput(int input)
{
	int exitCode = NOT_HANDLED;
	if (input == KEY_MOUSE) //input could be delivered to any control
	{
		//find control being operated on
		Registration* r = findMouseInputRecipient();
		if (r == nullptr)
			return exitCode;

		exitCode = handleMouseInput(input, r);
	}
	else if (focusedReg->listen_map & KEY_LISTENER)//keyboard input goes directly to focused control only, which is capable of handling key input
	{
		exitCode = handleKeyInput(input, focusedReg);
	}
	return exitCode;
}

Registration* ControlManager::findMouseInputRecipient()
{
	for (auto it = controls.rbegin(); it != controls.rend(); it++) //we start from the end so highest overlapping windows are processed first
	{
		Registration* r = *it;
		Controllable* c = r->c;
		MEVENT event;
		nc_getmouse(&event);

		//control contains the mouse click, and can respond to mouse events
		if (wenclose(c->getWindow(), event.y, event.x) && (r->listen_map & MOUSE_LISTENER))
			return r;
	}

	return nullptr;
}


//int ControlManager::handleControlInput(int input)
//{
//	int handled = NOT_HANDLED;
//	//process input through registered controls
//
//	//TODO why are we iterating through all the controls? We should just process the focused one!
//	for (auto it = controls.rbegin(); it != controls.rend(); it++) //we start from the end, so modal windows are always processed first
//	{
//		Registration* r = *it;
//		bool isModal = r->c->isModal();
//
//		if (isModal)
//		{
//			if (r->cmd == nullptr) //no point in checking registration with no callback
//				break; //modal window cannot handle the input
//		}
//		else if (r->cmd == nullptr) //non-modal window cannot handle the input, so we will check the next
//			continue;
//		
//		if (input == KEY_MOUSE && (r->listen_map & MOUSE_LISTENER))
//			handled = handleMouseInput(input, r);
//		else if (input != KEY_MOUSE && r->listen_map & KEY_LISTENER)
//			handled = handleKeyInput(input, r);
//
//		if (handled >= 0 || (handled == NOT_HANDLED && isModal))
//			break;
//	}	
//	return handled;
//}

int ControlManager::handleKeyInput(int input, Registration* r)
{
	if(focusedReg != r)
		return NOT_HANDLED;

	return r->cmd->execute(r->c, input);
}

int ControlManager::handleMouseInput(int input, Registration* r)
{
	int handled = NOT_HANDLED;
	Controllable* c = r->c;
	MEVENT event;
	nc_getmouse(&event);

	if (wenclose(c->getWindow(), event.y, event.x))
	{
		setFocus(c);		
		handled = r->cmd->execute(c, input);
	}

	return handled;
}

bool ControlManager::shutdown()
{
	if (!active)
	{
		//pop all controls
		controls.clear();
		globalShortcuts.clear();
		return true;
	}
	else
		return false;
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



