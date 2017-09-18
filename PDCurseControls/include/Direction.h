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

const std::vector<Dir> allDirs({ Dir::UP, Dir::DOWN, Dir::LEFT, Dir::RIGHT});

Dir getOppositeDir(Dir dir);