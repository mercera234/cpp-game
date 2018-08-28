#pragma once
#include <vector>

//TODO consider renaming this file to Motion.h
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

const std::vector<Dir> allDirs({ Dir::UP, Dir::DOWN, Dir::LEFT, Dir::RIGHT});

Dir getOppositeDir(Dir dir);
Axis getAxis(Dir dir);