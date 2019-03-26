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

WINDOW* WindowManager::newWin(Rect& rect)
{
	return newWin(rect.height, rect.width, rect.origin.y, rect.origin.x);
}

WINDOW* WindowManager::deriveWin(WINDOW* baseWin, Rect& rect)
{
	Pos derivedPos(rect.origin.y + getbegy(baseWin), rect.origin.x + getbegx(baseWin));
	
	Rect derivedRect = rect;
	derivedRect.origin = derivedPos;

	return newWin(derivedRect);
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