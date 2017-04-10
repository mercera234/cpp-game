#include "Map.h"
#include <fstream>

Map::Map(string name, int rows, int cols, WINDOW* win)
{
	id = 0;
	this->name = name;
	this->totalRows = rows;
	this->totalCols = cols;
	totalTiles = totalRows * totalCols;
	effectsLayer = new short[totalTiles];

	eLayer = new _2DStorage<int>(totalRows, totalCols);
	display = new Image(rows, cols, win); //use same window as map

	brightness = true;
	focusable = true;

	reset();

	setWindow(win);
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

	display->load(&gFile);
	
//	gFile.read((char*)effectsLayer, sizeof(short) * totalTiles);

	gFile.close();
}


/*
Works, 
and resizing to fixed sizes instead of exactly what we specify
	The map object will need to modify to account for this as well
*/
void Map::resize(int rows, int cols)
{
	this->totalRows = rows;
	this->totalCols = cols;

	int prevTotalTiles = totalTiles;
	totalTiles = rows * cols;

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
	delwin(win);
	delete effectsLayer;
	
	delete eLayer;
	delete display;
}