#include "MapEffectFilterPattern.h"
#include "ChtypeFilter.h"
#include "TUI.h"


void MapEffectFilterPattern::applyFilterPattern()
{
	Map* m = (Map*)c;

	//add filter to map
	for (int row = 0; row < m->getTotalRows(); row++)
	{
		for (int col = 0; col < m->getTotalCols(); col++)
		{
			//translate map coordinates to window coords
			int trueRow = row - m->getUlY();
			int trueCol = col - m->getUlX();

			if (trueRow < 0 || trueCol < 0 || trueRow >= m->getVisibleRows() || trueCol >= m->getVisibleCols())
				continue; //only manipulate character if onscreen

			chtype ch = mvwinch(win, trueRow, trueCol);

			//verify that filter applies to this tile
			EffectType effectType = m->getEffectsLayer().getDatum(row, col);
			
			if (effectType == EffectType::NONE)
			{
				//no effect so use black and white
				ch = ChtypeFilter::applyBWFilter(ch);
			}
			else
			{
				//use color filter
				short color = getColor(effectType);
				ch = ChtypeFilter::applyMarkFilter(ch, color, '#');
			}

			waddch(win, ch); //reapply filtered colors
		}
	}
}

short MapEffectFilterPattern::getColor(EffectType effect)
{
	short color;
	switch (effect)
	{ //hard code all colors for effects here
	case EffectType::OBSTR: color = COLOR_RED_BOLD; break;
	case EffectType::JUMPABLE: color = COLOR_CYAN_BOLD; break;
	case EffectType::HP_ALTER: color = COLOR_MAGENTA; break;
	case EffectType::DOWN_EXIT:
	case EffectType::UP_EXIT: color = COLOR_GREEN_BOLD; break;
	default: color = COLOR_YELLOW_BOLD; break;
	}

	return color;
}
