#pragma once

#include <iostream>
#include "curses.h"
#include "Storable.h"

//character types
#define AT_HUMAN 0
#define AT_CPU 1
//#define CHAR_NPC 1



//maximums
const int MAX_LEVELS = 99;
const int MAX_PLAYER_HP = 9999;
const int MAX_EXP = 9999999;
const int MAX_PLAYERS = 4;

const int MAX_NAME_LENGTH = 16; //max characters in string

//statuses
#define ALIVE 0
#define REVEALED 1
#define IS_ALIVE(a) (a->currHp > 0) //takes a pointer to an actor
#define IS_DEAD(a) (a->currHp <= 0) //takes a pointer to an actor

//effect results (this shouldn't be here!)
#define F_ESCAPE_FIGHT 0
#define F_SUCCESS 1
#define F_FAILED 2

//attributes - (these can now be used in the script, although we're likely to only use hp and exp)
enum 
{
	EXP = 0,
	HP,
	STRENGTH
};


/*
ActorDef - a struct containing a character blueprint
All ActorDefs will be originally stored in a file and only one copy of each is read into memory
This is everything that would be setup in an editor for a character
*/
struct ActorDef : public Storable
{
    std::string name;
	
	//changed symbol from char to int because chars are padded out to 4 bytes in files
	//	this changes nothing about its usage in code anyway
	chtype symbol; //appearance of character
	
	unsigned short level;
	unsigned int exp;
	unsigned int money;

    unsigned int maxHp;
	unsigned int maxMp;
	
    unsigned short strength;

	/*a percentage(based on 256) of how much physical damage is absorbed 
	Example:
	0 take all damage
	128 take half damage
	256 take no damage
	*/
	unsigned short defense; 

	unsigned short intelligence;

	/*a percentage(based on 256) of how much magic damage is absorbed
	Example:
	0 take all damage
	128 take half damage
	256 take no damage
	*/
	unsigned short will;

	unsigned short agility;
	float accuracy;
	float luck;

	//do this differently!
	int skills; //each bit represents 1 ability so, there are a max of 32
	
	//int weapon; //number is a reference to weapon in MIL
    //armor
	//item
	
	//int ailments; //not sure if this should be with Actor or ActorDef
	//bool save(ofstream* saveFile);
	int save(std::ofstream& saveFile);
	int load(std::ifstream& loadFile);
};

/*
Actor - An instantiation of a CharDef. A copy of a CharDef that can be manipulated until we are ready to dispose of it
*/
struct Actor
{
	unsigned int defIndex; //index of chardefinition
	ActorDef* def; //actual definition

	//CharICS* charICS; //handles inventory for character
    short type;
    int currHp;
	int currMp;
    short x, y;//relative to map window
	short prevX, prevY; //previous position
	/*bool save(ofstream* saveFile);
	bool load(ifstream* loadFile);*/
};



