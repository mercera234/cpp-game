#include <fstream>
#include <sstream>
#include "MapRoom.h"
#include "TUI.h"

//const std::string imagePiece = "img";
//const std::string effectPiece = "eff";

MapRoom::MapRoom(const std::string& name, int rows, int cols, WINDOW* win) :
	display(rows, cols, win) //Image uses same window as map, but the Image is responsible for destroying it
{
	this->name = name;

	Controllable::setWindow(win);

	setDimensions(rows, cols);
}


//MapRoom::MapRoom(WINDOW* win, const std::string& fileName)
//{
//	Controllable::setWindow(win);
//
//	/*TODO std::ifstream is(fileName);
//	load(is);*/
//}

void MapRoom::setWindow(WINDOW* win)
{
	Controllable::setWindow(win);
	display.setWindow(win);
}


void MapRoom::setDimensions(unsigned int rows, unsigned int cols)
{
	Controllable::setDimensions(rows, cols);
	display.setDimensions(rows, cols);
	effectsLayer.setDimensions(rows, cols);
}


void MapRoom::reset()
{
	display.reset();
	effectsLayer.fill(EffectType::NONE);
}


void MapRoom::draw()
{
	display.draw();	

	//draw map things
	for each (Sprite* thing in sprites)
	{
		for (int i = 0; i < thing->height; i++)
		{
			for (int j = 0; j < thing->width; j++)
			{
				TUI::printOnBkgd(thing->symbol, win, thing->pos.y + i, thing->pos.x + j);
			}
		}
		
	}
	wnoutrefresh(win);
}

void MapRoom::save(std::ofstream& saveFile)
{
	//save only graphical details that cannot be easily stored in text file
	display.save(saveFile);
	effectsLayer.save(saveFile);
}

void MapRoom::load(std::ifstream& loadFile)
{
	if(display.getWindow() == nullptr)
		display.setWindow(win);

	display.load(loadFile); //this maybe shouldn't be here. This along with the effects layer will be costly loads especially if multiple maps are present in game
		//this could be loaded only when needed
	
	
	setDimensions(display.getTotalRows(), display.getTotalCols());
	effectsLayer.load(loadFile);
}


void MapRoom::setPosition(int y, int x)
{
	//set position for both map and image
	Controllable::setPosition(y, x);
	display.setPosition(y, x);
}


void MapRoom::resize(int rows, int cols)
{
	int prevTotalTiles = totalTiles;

	setDimensions(rows, cols);
	display.setDimensions(rows, cols);
	effectsLayer.setDimensions(rows, cols, EffectType::NONE);
}

Sprite* MapRoom::checkCollisionDetection(Pos& pos)
{
	for (Sprite* sprite : sprites)
	{
		if (pos.y >= sprite->pos.y && pos.y < sprite->pos.y + sprite->height
			&& pos.x >= sprite->pos.x && pos.x < sprite->pos.x + sprite->width)//if item is stepped over
		{
			return sprite;
		}
	}
	return nullptr;
}

MapRoom::~MapRoom()
{	
	
}