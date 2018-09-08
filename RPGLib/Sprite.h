#pragma once
#include "Direction.h"
#include "curses.h"
#include "Thing.h"

/*Any instance of a game object that is rendered over a map room.*/
struct Sprite
{
	/*A ptr to the thing. This thing will live in a separate repository.
	There should be only one instance of this particular thing.*/
	Thing* thing;

	/*The symbol representing this thing on a map.
	If background color is set, it will be disregarded.*/
	chtype symbol;

	/*The placement of the thing in a map room.*/
	Pos pos;


	int quantity;

	Sprite();
	~Sprite();
};


