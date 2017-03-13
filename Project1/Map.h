#pragma once

#include "curses.h"
#include "Controllable.h"
#include <iostream>
using namespace std;
#include "2DStorage.h"
#include "Image.h"


#define NULL_MARKER_SPACING 8

#define E_NONE 0
#define E_OBSTR 1
#define E_JUMPABLE 2
#define E_HP_ALT_CONST 3
//#define E_HP_ALT_INC 4 //this idea just got killed! How to store HP alt amount in a short and track it while moving?
//#define E_AILMENT 0x0010
//#define E_SAVEABLE 0x0020
#define E_EXIT 32767


typedef unsigned int mapEffect;
#define EFF_TYPE 0x0000ffff
#define EFF_VALUE 0xffff0000

//#define E_OBSTR 0x0001
//#define E_JUMPABLE 0x0002
//#define E_DMG_CONST 0x0004
//#define E_DMG_INC 0x0008
//#define E_AILMENT 0x0010
//#define E_SAVEABLE 0x0020
//#define E_EXIT 0x0040

/*
Effect value breakdown from MSB to LSB
	(0,1) value
	(2,3) effect - unsigned
*/

// MSB = value, LSB = effect bitmap

#define LAYER_DISPLAY 0
#define LAYER_EFFECT 1

class Map : public Controllable
{
private:
	//meta members
	string name;
	unsigned short id; 

	//dimensional members
	unsigned int totalTiles;
	
	//data storage
	Image* display;
	bool brightness; //true - bright, false - dark

	//saveable data
	short* effectsLayer;
	_2DStorage<int>* eLayer;

	
	
public:
	Map(string name, int rows, int cols, WINDOW* win);

	void draw();
	void reset(); //wipe all data out of the map
	void resize(int rows, int cols);

	Image* getDisplay() { return display; }
	short* getEffectsLayer() { return effectsLayer; }
	void setEffect(int y, int x, short effect);
	void removeEffect(int y, int x, short effect);
	short getEffects(int y, int x);
	_2DStorage<int>* getLayer(int layer);

	string getName() { return name; }

	bool save(string fileName);
	bool load(string fileName);
	~Map();
};