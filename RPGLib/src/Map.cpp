#include <fstream>
#include "Map.h"
#include "TUI.h"



/*
Create a new map from scratch
*/
Map::Map(const std::string& name, int rows, int cols, WINDOW* win) :
	display(rows, cols, win) //Image uses same window as map, but the Image is responsible for destroying it
{
	setDefaults();

	setWindow(win);

	this->name = name;

	setDimensions(rows, cols);

	//set hi-level map data
	unitsHigh = totalRows / unitHeight;
	unitsWide = totalCols / unitWidth;
	totalUnits = unitsHigh * unitsWide;

	unitMaps = new TwoDStorage<char>(unitsHigh, unitsWide);
	for (int i = 0; i < totalUnits; i++)
	{
		unitMaps->setDatum(i, (char)0x0); //
	}

	effectsLayer.setDimensions(rows, cols);
	effectsLayer.fill(EffectType::NONE);

	reset();
}


Map::Map(WINDOW* win, const std::string& fileName)
{
	setDefaults();

	setWindow(win);
	load(fileName);
}


void Map::setDefaults()
{
	id = 0;
	brightness = true;
	focusable = true;

	controlActor = nullptr;
}


void Map::reset()
{
	display.reset();
	effectsLayer.fill(EffectType::NONE);
}


void Map::draw()
{
	display.draw();

	if (controlActor != NULL) //draw actor if present
	{
		chtype normalColor = COLOR_PAIR(COLOR_YELLOW_BOLD);
		chtype mainCImageNormal = controlActor->def->symbol | normalColor;

		TUI::printOnBkgd(mainCImageNormal, win, controlActor->y - display.getUlY(), controlActor->x - display.getUlX());
		wnoutrefresh(win);
	}
}


bool Map::save(const std::string& fileName)
{
	std::ofstream gFile;
	gFile.open(fileName, std::ios::trunc | std::ios::binary);
	if (gFile.is_open() == false)
		return false;

	gFile.write((char*) &id, sizeof(short));

	display.save(gFile);
	
	std::vector<EffectType> v = effectsLayer.getData();

	std::for_each(v.begin(), v.end(), [&](EffectType& type) 
	{
		gFile.write((char*) &type, sizeof(EffectType));
	});

	gFile.close();
}

bool Map::load(const std::string& fileName)
{
	std::ifstream gFile;
	gFile.open(fileName, std::ios::binary);
	if (gFile.is_open() == false)
		return false;

	gFile.read((char*)&id, sizeof(short));


	if(display.getWindow() == nullptr)
		display.setWindow(win);

	display.load(gFile); //this maybe shouldn't be here. This along with the effects layer will be costly loads especially if multiple maps are present in game
		//this could be loaded only when needed
	
	setDimensions(display.getTotalRows(), display.getTotalCols());

	effectsLayer.setDimensions(totalRows, totalCols);
	for (int i = 0; i < totalTiles; i++)
	{
		EffectType type;
		gFile.read((char*)&type, sizeof(EffectType));

		effectsLayer.setDatum(i, type);
	}

	gFile.close();

	//! these parameters are not saved and loaded yet
	brightness = true;
	focusable = true;
}


void Map::setPosition(int y, int x)
{
	//set position for both map and image
	Controllable::setPosition(y, x);
	display.setPosition(y, x);
}


void Map::resize(int rows, int cols)
{
	int prevTotalTiles = totalTiles;

	setDimensions(rows, cols);
	display.setDimensions(rows, cols);
	effectsLayer.setDimensions(rows, cols, EffectType::NONE);
}



Map::~Map()
{	

}