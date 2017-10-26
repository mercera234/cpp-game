#include "image_tests.h"
#include "Image.h"
#include "Highlighter.h"

void imageTest()
{
	Image img;
	img.setWindow(stdscr);
	img.setDimensions(screenHeight, screenWidth);

	std::ifstream is;
	is.open("data\\itest.img", std::ios::binary);

	img.load(is);
	is.close();

	img.draw();

	doupdate();
	int c = getch();

	if (c == 's')
	{
		std::ofstream os;
		os.open("data\\itest.img", std::ios::trunc | std::ios::binary);
		img.save(os);
		os.close();
	}
}


void highlighterTest()
{
	int height = getmaxy(stdscr);
	int width = getmaxx(stdscr);

	int curY, curX;
	curY = height / 2;
	curX = width / 2;

	int totalTiles = height * width;

	Image image(height, width, stdscr);

	TwoDStorage<chtype>* tileMap = image.getTileMap();
	//chtype* tileMap = new chtype[totalTiles];
	char asciiStart = ' ';
	char asciiEnd = '~';
	char asciiPtr = asciiStart;
	for (int i = 0; i < totalTiles; i++)
	{
		tileMap->setDatum(i, (chtype)asciiPtr++);

		if (asciiPtr >= asciiEnd)
			asciiPtr = asciiStart;
	}

	Highlighter* hl = new Highlighter(&image, &curY, &curX);

	bool playing = true;
	
	while (playing)
	{
		clear();

		for (int i = 0; i < totalTiles; i++)
		{
			int tileY = i / width;
			int tileX = i % width;
			chtype c = tileMap->getDatum(i);
			
			mvaddch(tileY, tileX, c);
		
		}

		mvaddch(curY, curX, '@');
		wnoutrefresh(stdscr);
		hl->draw();

		doupdate();
		int input = getch();
		
		switch(input)
		{
		case KEY_ESC: playing = false; break;
		case KEY_LEFT: curX--;hl->setHighlighting(false); break;
		case KEY_RIGHT: curX++; hl->setHighlighting(false); break;
		case KEY_UP: curY--; hl->setHighlighting(false); break;
		case KEY_DOWN: curY++; hl->setHighlighting(false); break;
		case KEY_SLEFT: hl->setHighlighting(true); curX--; break;
		case KEY_SRIGHT: hl->setHighlighting(true); curX++; break;
		case KEY_SUP: hl->setHighlighting(true); curY--; break;
		case KEY_SDOWN: hl->setHighlighting(true); curY++; break;
		case CTRL_C: 
			hl->copy();
			hl->setHighlighting(false);	
			break;
		case CTRL_V:
			hl->paste();
			break;
		case KEY_DC: //delete highlit region			
			hl->erase();
			break;
		case CTL_UP:
		case CTL_DOWN:
			hl->flip(AXIS_VER);
			break;
		case CTL_LEFT:
		case CTL_RIGHT:
			hl->flip(AXIS_HOR);
			break;
		default: //fill with printable character
			hl->fill(input);
			break;
		}




	}



}