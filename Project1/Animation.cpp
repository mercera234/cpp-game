#include "Animation.h"

void Animation::setPlayDirection(bool direction)
{
	playDirection = direction;
	prepare();
}