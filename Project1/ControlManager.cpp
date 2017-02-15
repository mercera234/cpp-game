#include "ControlManager.h"
#include "curses.h"
#include "MouseHelper.h"
#include <iterator>

void ControlManager::registerControl(Controllable* c, char listeners, void(*callback) (void*, void*, int))
{
	Registration* r = new Registration();

	r->c = c;
	c->setControlManager(this);
	r->callback = callback;
	r->listen_map = listeners;
	controls.push_back(r);
}

void ControlManager::registerShortcutKey(int key, void(*callback) (void*, void*, int))
{
	KeyAccelerator* shortcut = new KeyAccelerator();
	shortcut->key = key;
	shortcut->callback = callback;
	shortcuts.push_back(shortcut);
}

void ControlManager::popControl()
{
	controls.pop_back();
	
	//set focus to the next focusable component in the stack
	list<Registration*>::reverse_iterator it;
	for (it = controls.rbegin(); it != controls.rend(); it++) //we start from the end, so modal windows are always processed first
	{
		Registration* r = *it;
		Controllable* c = r->c;

		if (c->isFocusable())
		{
			focus = c;
		}
	}
}


Controllable* ControlManager::getFocus()
{
	return focus;
}

bool ControlManager::handleGlobalInput(int input)
{
	//process global input for non-modal controls
	list<KeyAccelerator*>::iterator it;
	for (it = shortcuts.begin(); it != shortcuts.end(); it++)//having this here suggests that global inputs cannot ever be used in local components
	{
		KeyAccelerator* k = *it;

		if (input == k->key)
		{
			k->callback(caller, this, input);

			if (shutdown)
			{
				//pop all controls
				controls.clear();
				return false;
			}
			return true;
		}
	}
	return true;
}

bool ControlManager::handleInput(int input)
{
	//process global inputs if the focused control is not modal
	if (focus->isModal() == false)
	{
		bool status = handleGlobalInput(input);
		if (status == false)
			return status;
	}

	list<Registration*>::reverse_iterator it;
	for (it = controls.rbegin(); it != controls.rend(); it++) //we start from the end, so modal windows are always processed first
	{
		Registration* r = *it;

		if (input == KEY_MOUSE && (r->listen_map & MOUSE_LISTENER))
		{
			Controllable* c = r->c;
			MEVENT event;
			nc_getmouse(&event);

			if(wenclose(c->getWindow(), event.y, event.x))
			{
				if (c->isFocusable())
					focus = c;

				if (r->callback != NULL)
				{
					r->callback(caller, c, input);
					break;
				}
					
			}
		}
		else if(r->listen_map & KEY_LISTENER)
		{
			Controllable* c = r->c;
			
			//process local key input			
			if (c == focus) //only route the input if control has focus
			{
				if (r->callback != NULL)
				{
					r->callback(caller, c, input);
				}
			}			
			break;
		}
	}


	if (shutdown)
	{
		//pop all controls
		controls.clear();
		return false;
	}
	else
		return true;
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
}