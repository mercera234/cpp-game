#include "ControlLayer.h"

void ControlLayer::registerControl(Controllable* c)
{
	controls.push_back(c);
}

void ControlLayer::registerMouseListener(Controllable* c, void(*callback)(void* ptr, int input))
{
	Registration* r = new Registration();

	r->c = c;
	r->callback = callback;
	mouseList.push_back(r);
}

void ControlLayer::registerKeyListener(Controllable* c, void(*callback) (void* ptr, int input))
{
	Registration* r = new Registration();

	r->c = c;
	r->callback = callback;
	keyList.push_back(r);
}



void ControlLayer::handleInput(int input, ControlManager* ptr)
{
	list<Registration*>::iterator it;
	switch (input) //first determine the source of input
	{
	case KEY_MOUSE: //cycle through mouse registered components and execute its callback function

		for (it = mouseList.begin(); it != mouseList.end(); it++)
		{
			Registration* r = *it;
			Controllable* c = r->c;
			MEVENT event;
			nc_getmouse(&event);

			if (c->isInWindow(event.y, event.x)) //determine if mouse event occurred inside this Controllable component
			{
				if (event.bstate == BUTTON1_CLICKED) //use the appropriate callback based on the mouse event
				{
					r->callback(ptr, input); //TODO the pointer that is passed in should be the registered control
					break; //once consumed we exit the loop
				}
			}
		}

		break;
	default: break;
	}
}


void ControlLayer::draw()
{
	list<Controllable*>::iterator it;
	for (it = controls.begin(); it != controls.end(); it++)
	{
		Controllable* c = *it;
		c->draw();
	}
}