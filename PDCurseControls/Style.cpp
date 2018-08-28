#include "Style.h"

int getPosition(Justf justf, int displayLength, int itemLength)
{
	int pos;
	switch (justf)
	{
	case Justf::LEFT: pos = 0; break;
	case Justf::RIGHT:
	{
		pos = displayLength - itemLength;
	} break;
	case Justf::CENTER:
	{
		int totalSpace = displayLength - itemLength;
		pos = totalSpace / 2;
	}
	}

	return pos;
}