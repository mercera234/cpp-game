#pragma once
#include "Image.h"

#define PLAY_FORWARD true
#define PLAY_BACKWARD !(PLAY_FORWARD)

class Animation : public Controllable
{
protected:
	unsigned int speed; //in milliseconds
	Image* background; //the image to be animated
	bool playDirection; //true if played forward
	bool playing;
	virtual void prepare() = 0; /*Prepare object for animating*/
public:
	virtual void frame() = 0;
	void setPlayDirection(bool direction);
	void play() { playing = true; } //animation will begin drawing
	bool isPlaying() { return playing; }
	void setSpeed(unsigned int speed) { this->speed = speed; }
};
