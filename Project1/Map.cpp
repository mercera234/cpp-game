#include "Map.h"
#include <fstream>
#include "TUI.h"

/*
Create a new map from scratch
*/
Map::Map(string name, int rows, int cols, WINDOW* win)
{
	this->name = name;

	setDimensions(rows, cols);

	effectsLayer = new short[totalTiles];

	eLayer = new _2DStorage<int>(totalRows, totalCols);
	display = new Image(rows, cols, win); //use same window as map

	//set hi-level map data
	unitsHigh = totalRows / unitHeight;
	unitsWide = totalCols / unitWidth;
	totalUnits = unitsHigh * unitsWide;

	unitMaps = new _2DStorage<char>(unitsHigh, unitsWide);
	for (int i = 0; i < totalUnits; i++)
	{
		unitMaps->setDatum(i, (char)0x0); //
	}

	//
	reset();

	setWindow(win);

	setDefaults();
}


Map::Map(WINDOW* win, string fileName)
{
	setWindow(win);
	load(fileName);
}


void Map::setDefaults()
{
	id = 0;
	brightness = true;
	focusable = true;

	controlActor = NULL;
}

_2DStorage<int>* Map::getLayer(int layer)
{
	_2DStorage<int>* strg = NULL;
	switch (layer)
	{
	case LAYER_EFFECT: strg = eLayer; break;
	}
	return strg;
}

/*
Clear the map (set all tiles to spaces)
Clear all effects from effect layer.
*/
void Map::reset()
{
	for (int row = 0; row < totalRows; row++)
	{
		for (int col = 0; col < totalCols; col++)
		{
		/*	chtype c = (chtype)(row % NULL_MARKER_SPACING == 0 && 
							    col % NULL_MARKER_SPACING == 0) ? '!' : ' ';*/
			int element = row * totalCols + col;
	
			
	//		dLayer->setDatum(row, col, ' ');

			eLayer->setDatum(row, col, 0);

			effectsLayer[element] = 0; //zero out effects layer
		}
	}

	display->reset();
}

void Map::setEffect(int y, int x, short effect)
{
	int element = y * totalCols + x;
	effectsLayer[element] |= effect;
}

void Map::removeEffect(int y, int x, short effect)
{
	int element = y * totalCols + x;
	effectsLayer[element] &= ~effect;
}

short Map::getEffects(int y, int x)
{
	int element = y * totalCols + x;
	return effectsLayer[element];
}

void Map::draw()
{
	display->draw();

	if (controlActor != NULL) //draw actor if present
	{
		chtype normalColor = COLOR_PAIR(COLOR_YELLOW_BOLD);
		chtype mainCImageNormal = controlActor->def->symbol | normalColor;

		TUI::printOnBkgd(mainCImageNormal, win, controlActor->y - display->getUlY(), controlActor->x - display->getUlX());
		wnoutrefresh(win);
	}
}


bool Map::save(string fileName)
{
	ofstream gFile;
	gFile.open(fileName, ios::trunc | ios::binary);
	if (gFile.is_open() == false)
		return false;

	gFile.write((char*) &id, sizeof(short));

	display->save(&gFile);
	
//	gFile.write((char*)effectsLayer, sizeof(short) * totalTiles);

	gFile.close();
}

bool Map::load(string fileName)
{
	ifstream gFile;
	gFile.open(fileName, ios::binary);
	if (gFile.is_open() == false)
		return false;

	gFile.read((char*)&id, sizeof(short));

	if (display == NULL)
	{
		display = new Image(win);
	}
	display->load(&gFile); //this maybe shouldn't be here. This along with the effects layer will be costly loads especially if multiple maps are present in game
		//this could be loaded only when needed
	
	setDimensions(display->getTotalRows(), display->getTotalCols());
////	gFile.read((char*)effectsLayer, sizeof(short) * totalTiles);

	gFile.close();

	//! these parameters are not saved and loaded yet
	brightness = true;
	focusable = true;
}


void Map::setDimensions(int rows, int cols)
{
	this->totalRows = rows;
	this->totalCols = cols;

	totalTiles = rows * cols;
}

/*
Works, 
and resizing to fixed sizes instead of exactly what we specify
	The map object will need to modify to account for this as well
*/
void Map::resize(int rows, int cols)
{
	int prevTotalTiles = totalTiles;

	setDimensions(rows, cols);

	//for now we will resize to the preferred size, but later on we will only do dynamic resizing to particular sizes
	chtype* newDisplayLayer = new chtype[totalTiles];
	short* newEffectsLayer = new short[totalTiles];

	for (int i = 0; i < totalTiles; i++)
	{
		newDisplayLayer[i] = ' ';
		newEffectsLayer[i] = 0; //zero out effects layer
	}


	short* oldEffects = effectsLayer;

	chtype* dlOut = newDisplayLayer;

	short* elOut = newEffectsLayer;
	short* elIn = effectsLayer;

	//use the smaller of the two sizes
	int tilesToCopy = prevTotalTiles < totalTiles ? prevTotalTiles : totalTiles;

	for (int i = 0; i < tilesToCopy; i++)
	{
		*elOut++ = *elIn++;
	}


	effectsLayer = newEffectsLayer;

	delete oldEffects;
}



Map::~Map()
{
	delete effectsLayer;
	
	delete eLayer;
	delete display;
}