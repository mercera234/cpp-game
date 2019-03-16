#pragma once
#include "Direction.h"

struct Rectangle
{
	Pos origin; //upper left hand corner of rectangle
	int height; 
	int width;

	Rectangle() {};
	Rectangle(int height, int width, Pos pos)
	{
		setDimensions(height, width, pos);
	}

	void setDimensions(int height, int width, Pos pos)
	{
		origin = pos;
		this->height = height;
		this->width = width;
	}

} typedef Rect;