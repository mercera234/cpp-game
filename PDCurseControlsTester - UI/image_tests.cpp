#include "image_tests.h"
#include "Image.h"
#include "Highlighter.h"

void drawPlainImage()
{
	resize_term(30, 100);

	Image img;
	img.setWindow(newwin(10, 10, 1, 1));
	img.setDimensions(5, 5);

	int y = -1;
	int x = -1;
	img.setPosition(y, x);
	TwoDStorage<chtype>* tileMap = img.getTileMap();
	tileMap->fill('!' | COLOR_BLUE << BKGDCOLOR_OFFSET);

	bool playing = true;
	while (playing)
	{
		img.draw();
		doupdate();
		int c = getch();

		switch (c)
		{
		case KEY_DOWN: y--; break;
		case KEY_UP: y++; break;
		case KEY_LEFT: x++; break;
		case KEY_RIGHT: x--; break;
		case KEY_ESC: 
			playing = false;
			continue;
			break;
		}
		img.setPosition(y, x);
	}
	
}





void imageLoadUnloadTest()
{
	Image img;
	img.setWindow(stdscr);
	img.setDimensions(23, 51);

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

void imageWithoutWindowTest()
{
	Image img;
	img.setDimensions(23, 51);

	img.draw(); //without win it will draw nothing

	doupdate();
	getch();
}

void imageWithoutDimensionsTest()
{
	Image img;
	img.setWindow(stdscr);
	
	/*without dimensions set, it will draw 
	the bottom right hand corner of a box 
	in the upper left hand corner. 
	This is because it attempts to draw one tile out of bounds before quitting.*/
	img.draw(); 

	doupdate();
	getch();
}

void imageWithoutDataTest()
{
	Image img;
	img.setWindow(stdscr);
	img.setDimensions(23, 51);

	//technically draws a black screen. Set dimensions, also sets the dimensions on the tilemap and prefills it with spaces
	img.draw();

	doupdate();
	getch();
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