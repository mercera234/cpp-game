#include "MapEffectFilterPattern.h"
#include "ChtypeFilter.h"
#include "TUI.h"

//void MapEffectFilterPattern::setEffectColorPair(unsigned short color, short effect)
//{
//	this->color = color;
//	this->effectType = effect;
//}


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

			if (trueRow < 0 || trueCol < 0 || trueRow >= m->getTotalRows() || trueCol >= m->getTotalCols())
				continue; //only manipulate character if onscreen

			chtype ch = mvwinch(win, trueRow, trueCol);

			//verify that filter applies to this tile
			//short tileEffect = m->getEffects(row, col);
			short tileEffect = m->getLayer(LAYER_EFFECT)->getDatum(row, col);

			if (tileEffect == E_NONE)
			{
				//no effect so use black and white
				ch = ChtypeFilter::applyBWFilter(ch);
			}
			else
			{
				//use color filter
				color = getColor(tileEffect);
				ch = ChtypeFilter::applyMarkFilter(ch, color, '#');
			}

			waddch(win, ch); //reapply filtered colors
		}
	}
}

short MapEffectFilterPattern::getColor(short effect)
{
	short color;
	switch (effect)
	{ //hard code all colors for effects here
	case E_OBSTR: color = COLOR_RED_BOLD; break;
	case E_JUMPABLE: color = COLOR_CYAN_BOLD; break;
	case E_HP_ALT_CONST: color = COLOR_MAGENTA; break;
	//case E_HP_ALT_INC: color = COLOR_MAGENTA_BOLD; break;
	case E_EXIT: color = COLOR_GREEN_BOLD; break;
	default: color = COLOR_YELLOW_BOLD; break;
	}

	return color;
}
