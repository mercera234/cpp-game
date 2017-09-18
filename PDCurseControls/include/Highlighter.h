#pragma once

#include "TUI.h"
#include "Rectangle.h"
#include "Image.h"

//move this to direction.h
const bool AXIS_HOR = false;
const bool AXIS_VER = true;

/*
Used to highlight a region within an Image object.
This cannot extend Controllable because a highlighter cannot exist by itself as a control.
*/
class Highlighter : public Drawable
{
private:
	WINDOW* win; //memory not managed here
	bool highlighting;
	bool pinPushed;
	int piny, pinx; //location of pin (where highlighting originates)
	short* curY;
	short* curX; //location of cursor
	short* offY;
	short* offX;

	//temporary storage for copy/paste operations
	TwoDStorage<chtype> clipBoard;
	
	/*
	The image associated with highlighter. Needed only in the draw method to get the upper left offset of the controllable.
	*/
	Image* img;

	//the tilemap of data to be altered during highlighting processes. For now, this will also be the tileMap associated with img.
	TwoDStorage<chtype>* tileMap;

	//Returns a rectangle of the highlit region. The pointer needs to be deleted after usage.
	Rect* getHighlitRegion();
	void swap(int y1, int x1, int y2, int x2);
	void pushPin(int y, int x);
public:
	/*
	Construct highlighter for imgIn using y and x and coordinates for a cursor.
	*/
	Highlighter(Image* imgIn, short* y, short* x);
	
	//Erase the highlit region within image. Erasing is filling the highlit region with spaces.
	void erase();

	//Fill the highlit region, which in turn fills a portion of the referenced image
	void fill(chtype fillChar);

	//Copy the highlit region. This means copying data from the image's tileMap to the highlighter's clipboard
	void copy();

	//Paste the clipboard contents starting at the cursor.
	void paste();

	/*
	If highlighting is turned on then flip the highlit region horizontally or vertically based on axys.
	If highlighting is off, then paste the buffer contents and then flip it
	*/
	void flip(bool axys);

	void draw();

	//getters/setters
	bool isHighlighting() { return highlighting; }
	void setHighlighting(bool status);
};

