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

	/*We need to reserve ample space for the controls, 
	because vector iterators are invalidated when memory is reallocated behind the scenes.
	*/
	//controls.reserve(30); 
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

void ControlManager::registerControl(Controllable* c, char listeners, void(*callback) (void*, void*, int))
{
	Registration* r = new Registration();

	r->c = c;
	c->setControlManager(this);
	r->callback = callback;
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

void ControlManager::registerShortcutKey(int key, void(*callback) (void*, void*, int))
{
	auto shortcutMapping = std::make_pair(key, callback);
	globalShortcuts.insert(shortcutMapping);
}


void ControlManager::popControl()
{
	if(controls.back() == focusedReg) //if the control being popped is focused
	{
		focusedReg = nullptr;
		//if object was focused then set focus to next 
		for (auto it = ++controls.rbegin(); it != controls.rend(); it++)//, focusNdx--) //we start from the end, so modal windows are always processed first
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

bool ControlManager::handleGlobalInput(int input)
{
	//check for cycle key
	if (input == cycleKey || input == revCycleKey)
	{
		cycleFocus(input);
		return true;
	}

	bool handled = false;

	auto shortcutMapping = globalShortcuts.find(input);
	auto mapping = *shortcutMapping;
	auto callback = mapping.second; 
	callback(caller, this, input);
	handled = true;

	return handled;
}

bool ControlManager::isGlobalInput(int input)
{
	return input == cycleKey || input == revCycleKey || (globalShortcuts.count(input) > 0);
}

bool ControlManager::handleInput(int input)
{
	bool handled = false;
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

bool ControlManager::handleControlInput(int input)
{
	bool handled = false;
	//process input through registered controls
	for (auto it = controls.rbegin(); it != controls.rend(); it++) //we start from the end, so modal windows are always processed first
	{
		Registration* r = *it;
		bool isModal = r->c->isModal();

		if (isModal)
		{
			if (r->callback == NULL) //no point in checking registration with no callback
				break; //modal window cannot handle the input
		}
		else if (r->callback == NULL) //non-modal window cannot handle the input, so we will check the next
			continue;
		
		if (input == KEY_MOUSE && (r->listen_map & MOUSE_LISTENER))
		{
			handled = handleMouseInput(input, r);

			//if mouse input was handled then the control should gain focus if possible
		/*	if (handled && r->c->isFocusable())
			{
				setFocus(r->c);
			}*/
		}
		else if (input != KEY_MOUSE && r->listen_map & KEY_LISTENER)
		{
			handled = handleKeyInput(input, r);
		}

		if (isModal) //only process input for the top modal window
			handled = true;

		if (handled)
			break;
	}	
	return handled;
}

bool ControlManager::handleKeyInput(int input, Registration* r)
{
	if(focusedReg != r)
		return false;

	r->callback(caller, r->c, input);
	return true;
}

bool ControlManager::handleMouseInput(int input, Registration* r)
{
	bool handled = false;
	Controllable* c = r->c;
	MEVENT event;
	nc_getmouse(&event);

	if (wenclose(c->getWindow(), event.y, event.x))
	{
		r->callback(caller, c, input);
		handled = true;
	}

	//if mouse input was handled then the control should gain focus if possible
	if (handled && c->isFocusable())
	{
		setFocus(c);
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
		c->draw();
	}
	if(focusedReg != nullptr)
		focusedReg->c->setCursorFocus(); //execute the focused component's method for rendering focus
}

ControlManager::~ControlManager()
{
	for each (Registration* r in controls)
	{
		delete r;
	}
}



