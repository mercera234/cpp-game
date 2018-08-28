#pragma once
#include "Animation.h"
#include <vector>
#include "TUI.h"

#define BLIT true
#define NO_BLIT !(BLIT)

/*
An animation that gradually brings an image to a black screen (or reverse)
*/
class BlitAnimation : public Animation
{
protected:
	TwoDStorage<bool>* blitMap; //true for blit, false for image shows through
	
	bool blit; //either blit or no blit depending on direction of play
	int tilesChangedCount;

	void changeTile(unsigned short element);
	void changeTile(unsigned short row, unsigned short col);

	void drawBlitMap();
	void prepare(); //overridden
	void init(Image* background, bool playForwards = true);
public:
	~BlitAnimation();
	void draw(); //overridden
};