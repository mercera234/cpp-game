#include "ControlManager.h"
#include "curses.h"
#include <iterator>

ControlManager::ControlManager(void* caller)
{ 
	this->caller = caller; 
}

void ControlManager::registerControl(Controllable* c, char listeners, void(*callback) (void*, void*, int))
{
	Registration* r = new Registration();

	r->c = c;
	c->setControlManager(this);
	r->callback = callback;
	r->listen_map = listeners;

	controls.push_back(r);

	cycleKey = '\t'; //tab will be the default
	revCycleKey = KEY_BTAB; //this is shifted tab key

	if (controls.size() == 1)
		focus = controls.begin();
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

void ControlManager::registerShortcutKey(int key, void(*callback) (void*, void*, int))
{
	KeyAccelerator* shortcut = new KeyAccelerator();
	shortcut->key = key;
	shortcut->callback = callback;
	shortcuts.push_back(shortcut);
}

void ControlManager::popControl()
{
	if (*focus == controls.back()) //if the control being popped is focused
	{
		//if object was focused then set focus to next 
		list<Registration*>::reverse_iterator it;
		for (it = ++controls.rbegin(); it != controls.rend(); it++) //we start from the end, so modal windows are always processed first
		{
			Registration* r = *it;
			Controllable* c = r->c;

			if (c->isFocusable())
			{
				focus = --it.base();//we always need to subtract 1 since the base iterator is one before the reverse iterator
				break;
			}
		}
	}
	controls.pop_back();
}


Controllable* ControlManager::getFocus()
{
	return (*focus)->c;
}

void ControlManager::cycleFocus(short key)
{
	do //cycle to next control until we find one that is focusable
	{
		if (key == cycleKey)
		{
			focus++;
			if (focus == controls.end())
				focus = controls.begin();
		}
		else if (key == revCycleKey)
		{
			if (focus == controls.begin()) //if focus is before beginning
				focus = --controls.end();
			else
				focus--;
		}
	}
	while ((*focus)->c->isFocusable() == false);
}

/*
Maybe there's a better way to do this!
*/
void ControlManager::setFocus(Controllable* c)
{ 
	list<Registration*>::iterator it;
	for (it = controls.begin(); it != controls.end(); it++)
	{
		Registration* r = *it;
		if (r->c == c)
		{
			focus = it;
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

	//process global input for non-modal controls
	list<KeyAccelerator*>::iterator it;
	for (it = shortcuts.begin(); it != shortcuts.end(); it++)//having this here suggests that global inputs cannot ever be used in local components
	{
		KeyAccelerator* k = *it;

		if (input == k->key)
		{
			k->callback(caller, this, input);

			handled = true;
			if (handled)
				break;
		}
	}
	return handled;
}

bool ControlManager::handleInput(int input)
{
	bool handled = false;
	//process global inputs if the focused control is not modal
	if ((*focus)->c->isModal() == false)
	{
		handled = handleGlobalInput(input);
	}
	
	if(handled == false)
		handleControlInput(input);

	return active;
}

void ControlManager::handleControlInput(int input)
{
	//process input through registered controls
	list<Registration*>::reverse_iterator it;
	for (it = controls.rbegin(); it != controls.rend(); it++) //we start from the end, so modal windows are always processed first
	{
		Registration* r = *it;

		if (input == KEY_MOUSE && (r->listen_map & MOUSE_LISTENER))
		{
			Controllable* c = r->c;
			MEVENT event;
			nc_getmouse(&event);

			if (wenclose(c->getWindow(), event.y, event.x))
			{
				if (c->isFocusable())
					focus = --it.base();
			
				if (r->callback != NULL)
				{
					r->callback(caller, c, input);
					break;
				}

			}
		}
		else if (r->listen_map & KEY_LISTENER)
		{
			if(*focus == r)
			{
				if (r->callback != NULL)
				{
					r->callback(caller, r->c, input);
					break;
				}
				else if (r->cmd != NULL) //implement command if setup
				{
					r->cmd->execute();
					break;
				}
			}

		}
	}
}


/*
Shuts down the control manager only if deactivated
*/
void ControlManager::shutdown()
{
	if (!active)
	{
		//pop all controls
		controls.clear();
		shortcuts.clear();
	}	
}

void ControlManager::draw()
{
	//draw all layers from bottom to top
	list<Registration*>::iterator it;
	for (it = controls.begin(); it != controls.end(); it++)
	{
		Registration* r = *it;
		Controllable* c = r->c;
		c->draw();
	}
	(*focus)->c->setFocus(); //execute the focused component's method for rendering focus
}