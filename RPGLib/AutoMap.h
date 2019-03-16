#pragma once
#include <vector>
#include <list>
#include "MegaMap.h"

/*A control used for viewing a mega map.
Will display tiles differently depending on whether they are known or visited.*/
class AutoMap : public Controllable, public InputProcessor
{
private:
	MegaMap* currMap = nullptr; //pointer to the current map
	int viewLayer = 0; //the current layer in map being viewed

	void alterViewLayer(int amount);
public:
	AutoMap() {}
	
	void setCurrMap(MegaMap* mapIn);
	void setWindow(WINDOW* win); //override
	void reset();

	void setPosition(int y, int x);

	MegaMap* getCurrMap() { return currMap; }

	void processInput();
	void draw(); //override
};
