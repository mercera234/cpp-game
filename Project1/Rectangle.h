#pragma once

struct Rectangle
{
	short y;
	short x;
	unsigned short height;
	unsigned short width;

	Rectangle() {};
	Rectangle(short height, short width, short y, short x)
	{
		this->y = y;
		this->x = x;
		this->height = height;
		this->width = width;
	}
} typedef Rect;