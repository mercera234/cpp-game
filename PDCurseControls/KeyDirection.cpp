#include "KeyDirection.h"


int getMagnitudeFromKey(int key, Controllable* c)
{
	int magnitude = 0;
	int visibleRows = 0;
	int visibleCols = 0;
	if (c != nullptr)
		getmaxyx(c->getWindow(), visibleRows, visibleCols);

	switch (key)
	{
	case KEY_UP:
	case KEY_LEFT: magnitude = -1;
		break;
	case KEY_DOWN:
	case KEY_RIGHT: magnitude = 1;
		break;
	case KEY_PGUP: magnitude = -visibleRows; break;
	case KEY_PGDN: magnitude = visibleRows;	break;
		//left right paging
	case CTL_PGUP: magnitude = -visibleCols; break;
	case CTL_PGDN: magnitude = visibleCols; break;
		//left right full movement
	case KEY_HOME: magnitude = -(c->getTotalCols()); break;
	case KEY_END: magnitude = c->getTotalCols(); break;
		// up down full movement
	case KEY_SHOME: magnitude = -(c->getTotalRows()); break;
	case KEY_SEND: magnitude = c->getTotalRows(); break;
	}
	return magnitude;
}

Dir getDirectionFromKey(int key)
{
	Dir dir;
	switch (key)
	{
	case KEY_SHOME:
	case KEY_PGUP:
	case KEY_UP: dir = Dir::UP;
		break;
	case KEY_SEND:
	case KEY_PGDN:
	case KEY_DOWN: dir = Dir::DOWN;
		break;
	case KEY_HOME:
	case CTL_PGUP:
	case KEY_LEFT: dir = Dir::LEFT;
		break;
	case KEY_END:
	case CTL_PGDN:
	case KEY_RIGHT: dir = Dir::RIGHT;
		break;
	default:
		dir = Dir::UNKNOWN;
	}
	return dir;
}

Axis getAxisFromKey(int key)
{
	Axis axis = Axis::HORIZONTAL;
	switch (key)
	{
	case KEY_SHOME:
	case KEY_PGUP:
	case KEY_UP:
	case KEY_SEND:
	case KEY_PGDN:
	case KEY_DOWN: axis = Axis::VERTICAL;
		break;
	case KEY_HOME:
	case CTL_PGUP:
	case KEY_LEFT:
	case KEY_END:
	case CTL_PGDN:
	case KEY_RIGHT: axis = Axis::HORIZONTAL;
		break;
	}
	return axis;
}