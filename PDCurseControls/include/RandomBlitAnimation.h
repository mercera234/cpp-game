#pragma once
#include "BlitAnimation.h"

class RandomBlitAnimation : public BlitAnimation
{
private:
	std::vector<int> tileTracker; //data structure for all indices on screen to be or not be blitted out
	std::vector<int>::iterator tilesFront; //front of blits vector(convenience variable)
public:
	RandomBlitAnimation(Image* background, bool playForwards = true);
	void frame(); //overridden
	void prepare(); //overridden
};