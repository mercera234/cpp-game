#include "SpiralBlitAnimation.h"
#include <windows.h> //cannot include <synchapi.h> by itself!
#include "MovementProcessor.h"

SpiralBlitAnimation::SpiralBlitAnimation(Image* background, bool playForwards)
{
	//using the center for spiral will produce the most efficient animation (less attempts to draw outside screen edges)
	origY = screenHeight / 2;
	origX = screenWidth / 2;

	spiralDirection = CLOCKWISE;
	init(background, playForwards);
}


void SpiralBlitAnimation::prepare()
{
	BlitAnimation::prepare();
	curY = origY;
	curX = origX;

	stepsLeft = stepsToEdge = 0;
}


void SpiralBlitAnimation::frame()
{
	Sleep(speed);

	//process
	if (tilesChangedCount == totalScreenTiles)
	{
		playing = false;
	}
	else
	{
		takeStep();	
	}
	
	
}


//void SpiralBlitAnimation::takeStep_old()
//{
//	if (stepsToEdge == 0)
//	{
//		blitMap->setDatum(curY, curX, !blit);
//		tilesChangedCount++;
//
//		stepsLeft = ++stepsToEdge;
//		edgeDirection = DIR_RIGHT;
//	}
//	else
//	{
//		bool qualityStep = false; //spiral may have to go offscreen, so we must verify that an onscreen step was taken
//
//		while (qualityStep == false && tilesChangedCount > 0)
//		{
//			switch (edgeDirection)
//			{
//			case DIR_UP: curY--; break;
//			case DIR_DOWN: curY++; break;
//			case DIR_LEFT: curX--; break;
//			case DIR_RIGHT: curX++; break;
//			}
//
//			if (curY < screenHeight && curX < screenWidth) //only blit if still on screen
//			{
//				blitMap->setDatum(curY, curX, !blit);
//				tilesChangedCount++;
//				qualityStep = true;
//			}
//
//			stepsLeft--;
//
//			if (stepsLeft == 0) //change spiral edge being followed
//			{
//				switch (edgeDirection)
//				{
//				case DIR_UP: edgeDirection = DIR_RIGHT;
//					stepsLeft = ++stepsToEdge;
//					break;
//				case DIR_DOWN: edgeDirection = DIR_LEFT;
//					stepsLeft = ++stepsToEdge;
//					break;
//				case DIR_LEFT: edgeDirection = DIR_UP;
//					stepsLeft = stepsToEdge;
//					break;
//				case DIR_RIGHT: edgeDirection = DIR_DOWN;
//					stepsLeft = stepsToEdge;
//					break;
//				}
//			}
//		}
//
//		
//	}
//
//
//}


void SpiralBlitAnimation::takeStep()
{
	if (stepsToEdge == 0)
	{
		changeTile(curY, curX);
		changeTile(curY, curX + 1); //for every step left or right 2 tiles are changed
		
		stepsLeft = ++stepsToEdge;
		edgeDirection = DIR_RIGHT;
	}
	else
	{
		bool qualityStep = false; //spiral may have to go offscreen, so we must verify that an onscreen step was taken

		while (qualityStep == false && tilesChangedCount < totalScreenTiles)
		{
			switch (edgeDirection)
			{
			case DIR_UP: curY--; break;
			case DIR_DOWN: curY++; break;
			case DIR_LEFT: curX -= 2; break;
			case DIR_RIGHT: curX += 2; break;
			}

			if (curY >= 0 && curX >= 0 && curY < screenHeight && curX < screenWidth) //only blit if still on screen
			{
				changeTile(curY, curX);
				qualityStep = true;
			}


			if (curY >= 0 && curX + 1 >= 0 && curY < screenHeight && curX + 1 < screenWidth) //only blit if still on screen
			{
				changeTile(curY, curX + 1);
				qualityStep = true;
			}


			stepsLeft--;

			if (stepsLeft == 0) //change spiral edge being followed
			{
				switch (edgeDirection)
				{
				case DIR_UP: edgeDirection = DIR_RIGHT;
					stepsLeft = ++stepsToEdge;
					break;
				case DIR_DOWN: edgeDirection = DIR_LEFT;
					stepsLeft = ++stepsToEdge;
					break;
				case DIR_LEFT: edgeDirection = DIR_UP;
					stepsLeft = stepsToEdge;
					break;
				case DIR_RIGHT: edgeDirection = DIR_DOWN;
					stepsLeft = stepsToEdge;
					break;
				}
			}
		}


	}


}