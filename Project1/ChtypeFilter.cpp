#include "ChtypeFilter.h"

chtype ChtypeFilter::applyBWFilter(chtype ch)
{
	int textC = ch & 0x0f000000;
	int bkgdC = ch & 0xf0000000;

	//translate to filters
	int fTextC;
	int fBkgdC;

	if (bkgdC == 0x00000000)
	{
		fBkgdC = bkgdC;
		fTextC = 0x07000000; //black text -> goes to white with black bkgd!				
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
	return ch | fTextC | fBkgdC;
}

chtype ChtypeFilter::applyColorFilter(chtype ch, int color)
{
	//get color values
	int textC = ch & 0x0f000000;
	int bkgdC = ch & 0xf0000000;

	//translate to filters
	int fTextC;
	int fBkgdC;

	if (bkgdC == 0x00000000)
	{
		fBkgdC = bkgdC;
		fTextC = COLOR_PAIR(color); //black text -> goes to white with black bkgd!				
	}
	else if ((bkgdC & 0x80000000) != 0) //any bold color
	{
		fBkgdC = (color << 28) | 0x80000000; //set to bold version of color selected
		fTextC = 0x00000000; //black text
	}
	else //pair with regular bkgd color
	{
		fBkgdC = (color << 28) & 0x7fffffff;//set to dull version of color selected
		fTextC = 0x00000000; //black text
	}

	ch &= 0x0000ffff; //remove all color
	return ch | fTextC | fBkgdC;

}

chtype ChtypeFilter::applyMarkFilter(chtype ch, int color, char mark)
{
	ch = ChtypeFilter::applyBWFilter(ch);
	int fTextC = mark | (color << 24);
	ch &= 0xf0ff0000; //clear the text color and text

	return ch | fTextC;
}