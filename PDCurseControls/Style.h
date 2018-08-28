#pragma once

enum class Justification
{
	LEFT,
	CENTER,
	RIGHT
} typedef Justf;


int getPosition(Justf justf, int displayLength, int itemLength);
