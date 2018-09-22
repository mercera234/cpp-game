#pragma once
#include "BlitAnimation.h"
#include "Direction.h"

#define CLOCKWISE true
#define COUNTER_CLOCKWISE !(CLOCKWISE)
class SpiralBlitAnimation : public BlitAnimation
{
private:
	unsigned short origY, origX; //the center of the spiral
	short curY, curX; //the position being blitted/not blitted
	bool spiralDirection; //clockwise or counter clockwise
	Dir edgeDirection; 
	unsigned int stepsLeft;
	unsigned int stepsToEdge;

	void takeStep();
public:
	SpiralBlitAnimation(Image* background, bool playForwards = true);
	void frame(); //overridden
	void prepare(); //overridden
};