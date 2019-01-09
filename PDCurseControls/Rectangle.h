#pragma once
#include "Direction.h"

struct Rectangle
{
	Pos origin; //upper left hand corner of rectangle
	unsigned int height; 
	unsigned int width;

	Rectangle() {};
	Rectangle(unsigned int height, unsigned int width, Pos pos)
	{
		setDimensions(height, width, pos);
	}

	void setDimensions(unsigned int height, unsigned int width, Pos pos)
	{
		origin = pos;
		this->height = height;
		this->width = width;
	}

} typedef Rect;