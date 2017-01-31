#include "ControlManager.h"


void ControlManager::registerControl(Controllable* c)
{
	list.push_back(c);
	//focus = list.front();
}

Controllable* ControlManager::getFocus()
{
	return focus;
}

void ControlManager::cycleFocus()
{
	
}