#pragma once
#include "Controllable.h"
#include <list>
using namespace std;

class ControlManager
{
private:

	list<Controllable*> list;
	Controllable* focus; //the item with focus

public:

	void registerControl(Controllable* c);
	Controllable* getFocus();
	
	void cycleFocus();

};