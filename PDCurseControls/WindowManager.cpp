#include "WindowManager.h"



//WindowManager& WindowManager::getInstance()
//{
//	static WindowManager instance;
//
//	return instance;
//}


WINDOW* WindowManager::newWin(int rows, int cols, int y, int x)
{
	WINDOW* win = newwin(rows, cols, y, x);

	pool.push_back(win);

	return win;
}

WindowManager::~WindowManager()
{
	for each (WINDOW* win in pool)
	{
		delwin(win);
	}
}

void WindowManager::delWin(WINDOW* win)
{
	pool.remove(win);
	delwin(win);
}