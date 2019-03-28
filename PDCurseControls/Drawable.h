#pragma once
#include "curses.h"
/*
An interface for drawable objects that contain their own window information (like controls)
*/
struct Drawable
{
	virtual void draw() = 0;
};

/*
An interface for everything that is drawable in another window (for objects that don't contain their own window)
*/
struct DrawablePiece
{
	virtual void draw(WINDOW*) = 0;
};