#include "Direction.h"

Dir getOppositeDir(Dir dir)
{
	Dir returnDir = Dir::UNKNOWN;
	switch (dir)
	{
	case Dir::UP: returnDir = Dir::DOWN; break;
	case Dir::DOWN: returnDir = Dir::UP; break;
	case Dir::LEFT: returnDir = Dir::RIGHT; break;
	case Dir::RIGHT: returnDir = Dir::LEFT; break;
	}
	return returnDir;
}

Axis getAxis(Dir dir)
{
	return (dir == Dir::LEFT || dir == Dir::RIGHT) ? Axis::HORIZONTAL : Axis::VERTICAL;
}

Axis getPerpAxis(Axis axis)
{
	return (axis == Axis::VERTICAL) ? Axis::HORIZONTAL : Axis::VERTICAL;
}

int getMagnitudeFromKey(int key, Controllable* c)
{
	int magnitude = 0;
	int visibleRows;
	int visibleCols;
	if(c != nullptr)
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
	Axis axis;
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

//void addMoves(Vector& sumMove, Movement& move1, Movement& move2)
//{
//	Axis axis1 = getAxis(move1.dir);
//	Axis axis2 = getAxis(move2.dir);
//
//	if (axis1 == axis2)
//	{
//		int mag1, mag2;
//
//		switch (move1.dir)
//		{
//		case Dir::UP: mag1 = -move1.magnitude; break;
//		case Dir::DOWN: mag1 = move1.magnitude; break;
//		case Dir::LEFT: mag1 = -move1.magnitude; break;
//		case Dir::RIGHT: mag1 = move1.magnitude; break;
//		}
//
//		switch (move2.dir)
//		{
//		case Dir::UP: mag2 = -move2.magnitude; break;
//		case Dir::DOWN: mag2 = move2.magnitude; break;
//		case Dir::LEFT: mag2 = -move2.magnitude; break;
//		case Dir::RIGHT: mag2 = move2.magnitude; break;
//		}
//		
//		int totalMag = mag1 + mag2;
//		if (axis1 == Axis::HORIZONTAL)
//		{
//			sumMove.horzMove.magnitude = totalMag;
//			sumMove.vertMove.magnitude = 0;
//		}
//		else
//		{
//			sumMove.horzMove.magnitude = 0;
//			sumMove.vertMove.magnitude = totalMag;
//		}
//	}
//	else //moves are on separate axes and cannot be added
//	{
//		if (axis1 == Axis::HORIZONTAL)
//		{
//			sumMove.horzMove = move1;
//			sumMove.vertMove = move2;
//		}
//		else
//		{
//			sumMove.horzMove = move2;
//			sumMove.vertMove = move1;
//		}
//	}
//}