#include <fstream>
#include <sstream>
#include "MapRoom.h"
#include "TUI.h"


MapRoom::MapRoom(const std::string& name, int rows, int cols, WINDOW* win) :
	display(rows, cols, win) //Image uses same window as map, but the Image is responsible for destroying it
{
	this->name = name;

	Controllable::setWindow(win);

	setDimensions(rows, cols);
}


MapRoom::MapRoom(WINDOW* win, const std::string& fileName)
{
	Controllable::setWindow(win);

	std::ifstream is(fileName);
	load(is);
}

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
	for each (Sprite* thing in things)
	{
		TUI::printOnBkgd(thing->symbol, win, thing->pos.y, thing->pos.x);
		//mvwaddch(win, thing->pos.y, thing->pos.x, thing->symbol);
	}
	wnoutrefresh(win);
}


int MapRoom::save(std::ofstream& saveFile)
{
	/*std::ofstream gFile;
	gFile.open(fileName, std::ios::trunc | std::ios::binary);
	if (gFile.is_open() == false)
		return false;*/

//	gFile.write((char*) &id, sizeof(int));

	//save only graphical details that cannot be easily stored in text file
	display.save(saveFile);
	
	std::vector<EffectType> v = effectsLayer.getData();

	std::for_each(v.begin(), v.end(), [&](EffectType& type) 
	{
		saveFile.write((char*) &type, sizeof(EffectType));
	});

	saveFile.close();

	return true;
}

int MapRoom::load(std::ifstream& loadFile)
{
	/*std::ifstream gFile;
	gFile.open(fileName, std::ios::binary);
	if (gFile.is_open() == false)
		return false;*/

	//loadFile.read((char*)&id, sizeof(int));


	if(display.getWindow() == nullptr)
		display.setWindow(win);

	display.load(loadFile); //this maybe shouldn't be here. This along with the effects layer will be costly loads especially if multiple maps are present in game
		//this could be loaded only when needed
	
	setDimensions(display.getTotalRows(), display.getTotalCols());

	effectsLayer.setDimensions(totalRows, totalCols);
	for (unsigned int i = 0; i < totalTiles; i++)
	{
		EffectType type;
		loadFile.read((char*)&type, sizeof(EffectType));

		effectsLayer.setDatum(i, type);
	}

	loadFile.close();

	//! these parameters are not saved and loaded yet
	brightness = true;
	focusable = true;

	return true;
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
	for each (Sprite* thing in things)
	{
		if (pos.y == thing->pos.y && pos.x == thing->pos.x)//if item is stepped over
		{
			return thing;
		}
	}
	return nullptr;
}

MapRoom::~MapRoom()
{	

}