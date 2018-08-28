#include "BlitAnimation.h"

void BlitAnimation::init(Image* background, bool playForwards)
{
	this->background = background;
	playing = true;

	blitMap = new TwoDStorage<bool>(screenHeight, screenWidth);
	
	playDirection = playForwards;
	prepare();
	setSpeed(1); //default is 1
}

void BlitAnimation::prepare()
{
	blit = playDirection ? NO_BLIT : BLIT;
	tilesChangedCount = 0;
	blitMap->fill(blit); //by default image shows through
}

void BlitAnimation::draw()
{
	background->draw();
	drawBlitMap();
}

void BlitAnimation::drawBlitMap()
{
	for (unsigned int i = 0; i < blitMap->getSize(); i++)
	{
		if (blitMap->getDatum(i) == NO_BLIT) //nothing to draw if false (no blit)
			continue;

		//we're assuming for now that the animation will always cover the full screen
		int row = i / screenWidth;
		int col = i % screenWidth;
		mvwaddch(stdscr, row, col, ' ');
	}
	wnoutrefresh(stdscr);
}

void BlitAnimation::changeTile(unsigned short element)
{
	blitMap->setDatum(element, !blit);
	tilesChangedCount++;
}

void BlitAnimation::changeTile(unsigned short row, unsigned short col)
{
	blitMap->setDatum(row, col, !blit);
	tilesChangedCount++;
}

BlitAnimation::~BlitAnimation()
{
	delete blitMap;
}