#pragma once
#include "curses.h"
#include <list>

class WindowManager
{
private:
	//static WindowManager* instance;
	std::list<WINDOW*> pool;
	
public:
	WindowManager() {}
	~WindowManager();
	//static WindowManager& getInstance();
	
	WINDOW* newWin(int rows, int cols, int y, int x);
	void delWin(WINDOW* win);

	//setters/getters

	int getCount() { return (int)pool.size(); }

	
};

