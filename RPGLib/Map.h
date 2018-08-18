#pragma once

#include "curses.h"
#include "Controllable.h"
#include <iostream>
#include "TwoDStorage.h"
#include "Image.h"
#include "Actor.h"
#include "TileEffect.h"


const unsigned short unitHeight = 23;
const unsigned short unitWidth = 51;


//refactor to name ScreenMap?
class Map : public Controllable
{
private:
	//meta members
	std::string name;
	unsigned short id = 0; 

	//data storage
	Image display;
	bool brightness = true; //true - bright, false - dark

	TwoDStorage<EffectType> effectsLayer;
	//saveable data

	//high level members
	TwoDStorage<char>* unitMaps;
	unsigned short totalUnits; //product of unit width and height
		//upper left corner or hlm
	unsigned short posY;
	unsigned short posX;
		//measured in unitMaps
	unsigned short unitsWide;
	unsigned short unitsHigh;

	//overlay data (eventually this should be private)
	
	Actor* controlActor = nullptr; //the actor controlled by the player and moved around on the map

public:
	Map() {}
	Map(const std::string& name, int rows, int cols, WINDOW* win); //create new map
	Map(WINDOW* win, const std::string& fileName); //load map from file

	void setWindow(WINDOW* win);
	void setDimensions(unsigned int rows, unsigned int cols);
	/*Clear the map (set all tiles to spaces)
	Clear all effects from effect layer.*/
	void reset(); 

	/*Resize the map. Resizes the display and effects layer, but not the window itself.*/
	void resize(int rows, int cols);

	bool save(const std::string& fileName);
	bool load(const std::string& fileName);
	void draw();
	~Map();

	//getters/setters
	Image* getDisplay() { return &display; }
	TwoDStorage<EffectType>& getEffectsLayer() { return effectsLayer; }
	
	unsigned short getId() { return id; }
	void setId(unsigned short id) { this->id = id; }

	void setHighLevelPosition(unsigned short hlY, unsigned short hlX) { posY = hlY; posX = hlX; }
	unsigned short getPosY() { return posY; }
	unsigned short getPosX() { return posX; }

	unsigned short getUnitWidth() { return unitsWide; }
	unsigned short getUnitHeight() { return unitsHigh; }
	TwoDStorage<char>* getUnitMaps() { return unitMaps; }

	void setName(const std::string& name) { this->name = name; }
	std::string getName() { return name; }
	void setPosition(int y, int x);

	void setControlActor(Actor* actor) { controlActor = actor; }
	Actor* getControlActor() { return controlActor; }
};