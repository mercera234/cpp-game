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