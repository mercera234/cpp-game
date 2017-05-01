#pragma once

//#include "charICS.h"
//#include "item.h"
//#include "ability.h"
//#include "AI.h"
#include "curses.h"
#include <iostream>
using namespace std;
#include "Storable.h"

/*need a serial number assigning system where
 0-just pick some in the random range(the scripts couldn't handle a negative value)
 1-255 is reserved
 255-RAND_MAX are free game
 RAND_MAX = SIGNED_INT_MAX 2 billion something
 */
#define MAX_RESERVED 256 //also the lowest random serial number that can be generated
#define GenerateRandomSerialNo() (rand() % (RAND_MAX - MAX_RESERVED)) + MAX_RESERVED

//character types
#define AT_HUMAN 0
#define AT_CPU 1
//#define CHAR_NPC 1



//maximums
#define MAX_LEVELS 99
#define MAX_PLAYER_HP 9999
#define MAX_EXP 9999999
#define MAX_PLAYERS 4

#define MAX_NAME_LENGTH 16 //max characters in string

//statuses
#define ALIVE 0
#define REVEALED 1
#define IS_ALIVE(a) (a->currHp > 0) //takes a pointer to an actor
#define IS_DEAD(a) (a->currHp <= 0) //takes a pointer to an actor

//effect results
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

#define EASY

/*
CharDef - a struct containing a character blueprint
All CharDefs will be originally stored in a file and only one copy of each is read into memory
This is everything that would be setup in an editor for a character
*/
struct ActorDef : public Storable
{
    string name;
	
	//changed symbol from char to int because chars are padded out to 4 bytes in files
	//	this changes nothing about its usage in code anyway
	chtype symbol; //appearance of character
	
	unsigned short level;
	unsigned int exp;
	unsigned int money;

    unsigned int maxHp;
	unsigned int maxMp;
	
    unsigned short strength;

	/*a percentage of how much damage is absorbed 
	Example:
	0 take all damage
	50 take half damage
	100 take no damage
	*/
	unsigned short defense; 
	unsigned short intelligence;
	unsigned short will;
	unsigned short agility;
	float accuracy;
	float luck;

	int skills; //each bit represents 1 ability so, there are a max of 32
	
	//int weapon; //number is a reference to weapon in MIL
    //armor
	//item
	
	//int ailments; //not sure if this should be with Actor or ActorDef
	bool save(ofstream* saveFile);
	bool load(ifstream* loadFile);
};

/*
Actor - An instantiation of a CharDef. A copy of a CharDef that can be manipulated until we are ready to dispose of it
*/
struct Actor
{
	unsigned int defIndex; //index of chardefinition
	ActorDef* def; //actual definition

	//unsigned int serialNo; //unique id
	//CharICS* charICS; //handles inventory for character
    short type;
    short currHp;
	short currMp;
    short x, y;//relative to map window
	short prevX, prevY; //previous position
	/*bool save(ofstream* saveFile);
	bool load(ifstream* loadFile);*/
};



