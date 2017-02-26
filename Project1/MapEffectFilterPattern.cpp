#include "MapEffectFilterPattern.h"
#include "ChtypeFilter.h"

void MapEffectFilterPattern::setEffectColorPair(unsigned short color, short effect)
{
	this->color = color;
	this->effect = effect;
}


void MapEffectFilterPattern::applyFilterPattern()
{
	Map* m = (Map*)c;

	//add filter to map
	for (int row = 0; row < m->getHeight(); row++)
	{
		for (int col = 0; col < m->getWidth(); col++)
		{
			//translate map coordinates to window coords
			int trueRow = row - m->getUlY();
			int trueCol = col - m->getUlX();

			if (trueRow < 0 || trueCol < 0 || trueRow >= m->getTotalRows() || trueCol >= m->getTotalCols())
				continue; //only manipulate character if onscreen

			chtype ch = mvwinch(win, trueRow, trueCol);

			//verify that filter applies to this tile
			short tileEffect = m->getEffects(row, col);

			if ((tileEffect & effect) == 0)
			{
				//no effect so use black and white
				ch = ChtypeFilter::applyBWFilter(ch);
			}
			else
			{
				//use color filter
				//ch = ChtypeFilter::applyColorFilter(ch, color);
				ch = ChtypeFilter::applyMarkFilter(ch, color, '#');
			}

			waddch(win, ch); //reapply filtered colors
		}
	}
}

