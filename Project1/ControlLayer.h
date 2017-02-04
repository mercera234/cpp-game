#pragma once
#include "Controllable.h"
#include <list>
using namespace std;

struct Registration
{
	Controllable* c;
	void(*callback) (void*, int);

	Registration() {};
};

class ControlManager;
class ControlLayer
{
private:
	list<Controllable*> controls;
	list<Registration*> mouseList;
	list<Registration*> keyList;
	Controllable* focus; //the item with focus
public:
	void registerControl(Controllable* c);
	void registerMouseListener(Controllable* c, void(*callback) (void*, int));
	void registerKeyListener(Controllable* c, void(*callback) (void*, int));

	void handleInput(int input, ControlManager* ptr);
	void draw();
};