#pragma once
#include <vector>
#include "Controllable.h"


struct Position
{
	int y = -1;
	int x = -1;
	Position() {}
	Position(int yIn, int xIn) { y = yIn; x = xIn; }
} typedef Pos;

enum class Axis
{
	HORIZONTAL,
	VERTICAL
};

enum class Direction
{
	UNKNOWN,
	UP,
	DOWN,
	LEFT,
	RIGHT
};
typedef Direction Dir;

enum class Boundary
{
	NORTH = -1,
	SOUTH = 1,
	IN_BOUNDS = 0,
	WEST = -2,
	EAST = 2
};

const std::vector<Dir> allDirs({ Dir::UP, Dir::DOWN, Dir::LEFT, Dir::RIGHT });

Dir getOppositeDir(Dir dir);
Axis getAxis(Dir dir);
Axis getPerpAxis(Axis axis);
Direction getDirectionFromKey(int key);
Axis getAxisFromKey(int key);
int getMagnitudeFromKey(int key, Controllable* c = nullptr);


struct Movement
{
	Axis axis;
	int magnitude = 0;
	//Dir dir = Dir::UNKNOWN;

	Movement() {}
	/*Movement(Dir d, int mag) {
		dir = d; magnitude = mag;
	}*/
	Movement(Axis a, int mag) {
		axis = a; magnitude = mag;
	}

	Movement getOppositeMove()
	{
		Movement m;
		m.axis = axis;
		//m.dir = getOppositeDir(dir);
		m.magnitude = -magnitude;
		return m;
	}
};

/*A vector is the combination of 2 moves on separate axes*/
//struct Vector
//{
//	Movement horzMove;
//	Movement vertMove;
//
//	Vector() {}
//	Vector(int vertAmt, int horzAmt)
//	{
//		vertMove.dir = vertAmt > 0 ? Dir::DOWN : Dir::UP;
//		horzMove.dir = horzAmt > 0 ? Dir::RIGHT : Dir::LEFT;
//		
//		vertMove.magnitude = abs(vertAmt);
//		horzMove.magnitude = abs(horzAmt);
//	}
//};

//void addMoves(Vector& sumMove, Movement& move1, Movement& move2);

/*Not sure that I like this. It tracks all the moves, but it clearly isn't a subclass of Movement*/
//struct MovementChain
//{
//	std::vector<Movement> moves;
//};




