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

	//TODO sprite id? To id things in one room for example


	/*The symbol representing this thing on a map.
	If background color is set, it will be disregarded.
	If sprite is larger than 1 tile, then symbol is used to cover the whole sprite.*/
	chtype symbol;

	/*Impassible if the player is not permitted to step on it. 
	The player may still be able to push into it though.*/
	bool impassible;

	/*The placement of the thing in a map room.*/
	Pos pos;
	int height = 1;
	int width = 1;

	int quantity;

	Sprite();
	~Sprite();
};


