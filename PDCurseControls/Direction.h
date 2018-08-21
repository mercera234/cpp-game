#pragma once
#include <vector>

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