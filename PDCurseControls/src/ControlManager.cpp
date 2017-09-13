#include "ControlManager.h"
#include "curses.h"
#include <iterator>

const short defaultCycleKey = '\t';
const short defaultRevCycleKey = KEY_BTAB;

ControlManager::ControlManager(void* caller)
{ 
	this->caller = caller; 

	//set default cycle keys
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

	if (controls.size() == 1)
	{
		setFocusedControl(controls.begin());
	}
}

void ControlManager::registerShortcutKey(int key, void(*callback) (void*, void*, int))
{
	auto shortcutMapping = std::make_pair(key, callback);
	globalShortcuts.insert(shortcutMapping);
}

void ControlManager::popControl()
{
	if (*focusedControl == controls.back()) //if the control being popped is focused
	{
		//if object was focused then set focus to next 
		std::list<Registration*>::reverse_iterator it;
		for (it = ++controls.rbegin(); it != controls.rend(); it++) //we start from the end, so modal windows are always processed first
		{
			Registration* r = *it;
			Controllable* c = r->c;

			if (c->isFocusable())
			{
				setFocusedControl(--it.base());//we always need to subtract 1 since the base iterator is one before the reverse iterator
				break;
			}
		}
	}
	controls.pop_back();
}

void ControlManager::unsetFocus()
{
	(*focusedControl)->c->setFocus(false);
}

void ControlManager::setFocusedControl(std::list<Registration*>::iterator it)
{
	focusedControl = it;
	(*focusedControl)->c->setFocus(true);
}

Controllable* ControlManager::getFocus()
{
	return (*focusedControl)->c;
}

void ControlManager::cycleFocus(short key)
{
	if (controls.empty())
		return;

	//unset focus on current controllable
	unsetFocus();

	do //cycle to next control until we find one that is focusable
	{
		if (key == cycleKey)
		{
			focusedControl++;
			if (focusedControl == controls.end())
				focusedControl = controls.begin();
		}
		else if (key == revCycleKey)
		{
			if (focusedControl == controls.begin()) //if focus is before beginning
				focusedControl = --controls.end();
			else
				focusedControl--;
		}
	}
	while ((*focusedControl)->c->isFocusable() == false);

	(*focusedControl)->c->setFocus(true);
}

/*
Maybe there's a better way to do this!
*/
void ControlManager::setFocus(Controllable* c)
{ 
	//unset focus on currently focused controllable
	unsetFocus();

	//set focus on c
	std::list<Registration*>::iterator it;
	for (it = controls.begin(); it != controls.end(); it++)
	{
		Registration* r = *it;
		if (r->c == c)
		{
			setFocusedControl(it);
			break;
		}
	}
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
		if((*focusedControl)->c->isModal())
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
	std::list<Registration*>::reverse_iterator it;
	for (it = controls.rbegin(); it != controls.rend(); it++) //we start from the end, so modal windows are always processed first
	{
		Registration* r = *it;
		if (r->callback == NULL) //no point in checking registration with no callback
			continue;

		if (input == KEY_MOUSE && (r->listen_map & MOUSE_LISTENER))
		{
			handled = handleMouseInput(input, r);

			//if mouse input was handled then the appropriate control should gain focus if possible
			if (handled && r->c->isFocusable())
			{
				setFocusedControl(--it.base());
			}
		}
		else if (r->listen_map & KEY_LISTENER)
		{
			handled = handleKeyInput(input, r);
		}

		if (handled)
			break;
	}	
	return handled;
}

bool ControlManager::handleKeyInput(int input, Registration* r)
{
	if (*focusedControl != r) //only the focusedControl can accept the key input
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
	std::list<Registration*>::iterator it;
	for (it = controls.begin(); it != controls.end(); it++)
	{
		Registration* r = *it;
		Controllable* c = r->c;
		c->draw();
	}
	(*focusedControl)->c->setCursorFocus(); //execute the focused component's method for rendering focus
}


/*
! method is not being used yet and is now out of date!
*/
//void ControlManager::registerControl(Controllable* c, char listeners, Command* cmd)
//{
//	Registration* r = new Registration();
//
//	r->c = c;
//	c->setControlManager(this);
//	//r->callback = callback;
//	r->cmd = cmd;
//	r->listen_map = listeners;
//	controls.push_back(r);
//
//	if (focus == NULL) //set first item to focus by default
//		focus = r;
//}

