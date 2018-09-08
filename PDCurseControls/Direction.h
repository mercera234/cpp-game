#pragma once
#include <vector>



struct Position
{
	int y = -1;
	int x = -1;
	Position() {}
	Position(int yIn, int xIn) { y = yIn; x = xIn; }
} typedef Pos;

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

enum class Axis
{
	HORIZONTAL,
	VERTICAL
};

const std::vector<Dir> allDirs({ Dir::UP, Dir::DOWN, Dir::LEFT, Dir::RIGHT });

Dir getOppositeDir(Dir dir);
Axis getAxis(Dir dir);

//TODO create a ChainedMovement object that can track multiple movements
struct Movement
{
	int magnitude = 0;
	Dir dir = Dir::UNKNOWN;

	Movement() {}
	Movement(Dir d, int mag) {
		dir = d; magnitude = mag;
	}

	Movement getOppositeMove()
	{
		Movement m;
		m.dir = getOppositeDir(dir);
		m.magnitude = magnitude;
		return m;
	}
};

/*Not sure that I like this. It tracks all the moves, but it clearly isn't a subclass of Movement*/
struct MovementChain
{
	std::vector<Movement> moves;
};


