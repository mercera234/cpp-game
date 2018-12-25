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