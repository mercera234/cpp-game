#pragma once

#include "curses.h"
#include "Controllable.h"
#include <iostream>
#include "TwoDStorage.h"
#include "Image.h"
#include "Actor.h"
#include "TileEffect.h"
#include "Sprite.h"

const std::string mapRoomExt = "mrm";

class MapRoom : public Controllable, public Thing, public Storable
{
private:
	//data storage
	bool brightness = true; //true - bright, false - dark
	bool randomEncounters = true; //most are true

	Image display;
	TwoDStorage<EffectType> effectsLayer;
	//saveable data during game

	//overlay data

public:
	/*All things that can be on a map (
	pickups (items and money)
	actors)*/
	std::list<Sprite*> sprites;
	
	MapRoom() {}
	MapRoom(const std::string& name, int rows, int cols, WINDOW* win); //create new map
	//MapRoom(WINDOW* win, const std::string& fileName); //load map from file

	void setWindow(WINDOW* win);
	void setDimensions(unsigned int rows, unsigned int cols);
	/*Clear the map (set all tiles to spaces)
	Clear all effects from effect layer.*/
	void reset(); 

	/*Resize the map. Resizes the display and effects layer, but not the window itself.*/
	void resize(int rows, int cols);

	/*Save display and effects layer.*/
	void save(std::ofstream& saveFile); //override

	/*Load display and effects layer.*/
	void load(std::ifstream& loadFile); //override

	
	void draw();

	Sprite* checkCollisionDetection(Pos& pos);


	~MapRoom();

	//getters/setters
	Image& getDisplay() { return display; }
	ITwoDStorage<EffectType>& getEffectsLayer() { return effectsLayer; }

	void setBrightness(bool brightnessIn) { brightness = brightnessIn; }
	bool getBrightness() { return brightness; }

	void setRandomEncounters(bool randomEncountersIn) { randomEncounters = randomEncountersIn; }
	bool getRandomEncounters() { return randomEncounters; }

	void setPosition(int y, int x);
};