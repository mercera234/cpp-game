#include "BWFilter.h"

void BWFilter::applyFilterPattern()
{
	//add filter to map
	for (int row = 0; row < c->getTotalRows(); row++)
	{
		for (int col = 0; col < c->getTotalCols(); col++)
		{
			chtype ch = mvwinch(win, row, col); //colorless filter

												//get color values
			int textC = ch & 0x0f000000;
			int bkgdC = ch & 0xf0000000;

			//translate to filters
			int fTextC;
			int fBkgdC;

			if (bkgdC == 0x00000000)
			{
				fBkgdC = bkgdC;
				fTextC = 0x0f000000; //black text -> goes to white with black bkgd!				
			}
			else if ((bkgdC & 0x80000000) != 0) //any bold color
			{
				fBkgdC = 0xf0000000; //white bkgd
				fTextC = 0x00000000; //black text
			}
			else //pair with regular bkgd color
			{
				fBkgdC = 0x80000000;//set to gray bkgd
				fTextC = 0x00000000; //black text
			}

			ch &= 0x0000ffff; //remove all color
			waddch(win, ch | fTextC | fBkgdC); //reapply filtered colors
		}
	}
}