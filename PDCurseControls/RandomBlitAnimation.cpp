#include "RandomBlitAnimation.h"
#include <ctime>
#include <windows.h> //cannot include <synchapi.h> by itself!


RandomBlitAnimation::RandomBlitAnimation(Image* background, bool playForwards)
{
	init(background, playForwards);

	srand((unsigned int)time(NULL));
}

void RandomBlitAnimation::frame()
{
	Sleep(speed);

	//process
	if(tilesChangedCount == totalScreenTiles)
	{
		playing = false;
	}
	else
	{
		int randNo = rand() % tileTracker.size();
		int element = tileTracker[randNo];
		tilesFront = tileTracker.begin();
		tileTracker.erase(tilesFront + randNo);

		changeTile(element);
	}
}

void RandomBlitAnimation::prepare() //overridden
{
	BlitAnimation::prepare();
	for (int i = 0; i < totalScreenTiles; i++)
	{
		tileTracker.push_back(i);
	}
}