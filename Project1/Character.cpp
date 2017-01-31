//
//  character.cpp
//  basicgame
//
//  Created by Alex Mercer on 11/13/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include <sstream>
#include <cmath>
#include "character.h"
#include "gameApp.h"

//currently trying to replace the old-style linked list with the stl version

const int colWidth = 10; //this is the same column width used in default.stats(later we may want this value directly retrieved from stats file)

bool CharController::init(WindowGroup &wg, Item* MILIn, Ability* MALin, GameApp* appIn)
{
	assert(MAX_RESERVED < RAND_MAX);
	
	//init basic data
    app = appIn;
    numChars = enemyCount = npcCount = playerCount = 0;
    screen = &wg;
	
	head = NULL;
	tail = NULL;
	MIL = MILIn;
	MAL = MALin;
	stepCount = 0;
	newSerial = 0;
	
	levelDataReader.open(STATS_FILE);
	if(levelDataReader.is_open() == false)
	{
		logHandle.logData("Failed to load stats file.");
		return false;
	}
	
	//initialize MCL, this should be read in from a file later on(these are only defs by the way)
	ifstream mclFile;
	
	mclFile.open(MCL_FILE, ios::in | ios::binary);
	if(mclFile.is_open() == false)
	{
		logHandle.logData("Failed to load MCL file.");
		return false;
	}
	
	mclFile.seekg(0, ios::end);
	int length = (int)mclFile.tellg();
	mclFile.seekg(0, ios::beg); //set pointer back to beginning
	
	//subtracting 4 so that i can easily add new chardef members without having to change the mclfile

	int count = (int)(length / (sizeof(CharDef) - unaccounted));
	
	assert(count <= MAX_CHARS);//do not attempt to load in more characters than the MCL is allocated for
	
	for(int i = 0; i < count; i++)
	{
		mclFile.read((char*)&MCL[i], (sizeof(CharDef) - unaccounted));
	}
	mclFile.close();
	
	//seed the generator if nothing else has done it so far
	srand(time(NULL));
	
	return true;
}

unsigned int CharController::generateSerialNum()
{
	unsigned int serNo = GenerateRandomSerialNo();
	
	for(Character* c = head; c != NULL; c = c->next)
	{
		if (c->serialNo == serNo) //duplicate serNo found
		{
			serNo = GenerateRandomSerialNo();
			c = head; //restart search
		}
	}
	
	return serNo;
}


void CharController::push_front(int serialNo, int defIndex, int type, int y, int x)
{
	Character* newChar = new Character;
	
	unsigned int serNo;
	if(serialNo == 0)//allow serNo to be generated automatically
	{
		serNo = generateSerialNum();
	}
	else 
		serNo = serialNo;
	
	newChar->serialNo = serialNo;
	newChar->defIndex = defIndex;
	newChar->type = type;
	newChar->xPos = x;
	newChar->yPos = y;
	newChar->prevX = -1; //-1 will indicate that character has not taken a step yet
	newChar->prevY = -1;
	newChar->charICS = new CharICS();
	
	//load def from MCL
	memcpy(&(newChar->def), &MCL[defIndex], sizeof(CharDef));

	newChar->currHp = newChar->def.maxHp;
	
    numChars++;
	
	//mHead.push_front(*newChar);
	
	newChar->next = head;
	if(head != NULL)
		head->prev = newChar;
	else
		tail = newChar; //there is only 1 node in list for this to occur
		
    head = newChar; //save character in head
	
	switch (newChar->type) 
	{
		case CHAR_MC: enemyCount++; break;
		case CHAR_NPC: npcCount++; break;	
		case CHAR_PC: playerCount++; break;
	}
}

void CharController::push_back(int serialNo, int defIndex, int type, int y, int x) //add character to end of list rather than the front
{
	Character* newChar = new Character;
	
	unsigned int serNo;
	if(serialNo == 0)//allow serNo to be generated automatically
	{
		serNo = generateSerialNum();
	}
	else 
		serNo = serialNo;
		
	newChar->serialNo = serialNo;
	newChar->defIndex = defIndex;
	newChar->type = type;
	newChar->xPos = x;
	newChar->yPos = y;
	newChar->prevX = -1; //-1 will indicate that character has not taken a step yet
	newChar->prevY = -1;
	newChar->charICS = new CharICS();
	
	//load def from MCL
	memcpy(&(newChar->def), &MCL[defIndex], sizeof(CharDef));
	
	newChar->currHp = newChar->def.maxHp;
	
	numChars++;
	
	//link character to end of list
	
	//	mHead.push_back(*newChar);
	//newChar->next = head;
	newChar->prev = tail;
	if(tail != NULL)
		tail->next = newChar;
	else
		head = newChar;//both head and tail are null(empty list)
	
	tail = newChar; //save character in tail
		
	switch (newChar->type) 
	{
		case CHAR_MC: enemyCount++; break;
		case CHAR_NPC: npcCount++; break;	
		case CHAR_PC: playerCount++; break;
	}
}


void CharController::remove(int serialNo)
{
	Character* c = getCharacter(serialNo);//get character
	if(c == NULL)
		return;
	
	int type = c->type;
	int serNo = c->serialNo;
	
	//logic for deleting from list
	/*list<Character>::iterator it;
	
	for(it = mHead.begin(); it != mHead.end(); it++)
	{
		Character c = *it;
		if(c.serialNo == serialNo)
		{
			mHead.erase(it);
			break;//we don't expect instances in a list to have the same id
		}
	}*/
	
	//rewire pointers so character can be removed
	if(c->prev != NULL) //ptr is after head
		c->prev->next = c->next;
	else //head is being removed
		head = c->next;
	
	if(c->next != NULL)//ptr is before tail
		c->next->prev = c->prev;
	else
		tail = c->prev;
	
	//the book also sets the head to null if we're deleting only node in list, but this should be useless
	//	so I'm not doing it
	
	//null out characters pointers
	c->next = NULL;
	c->prev = NULL;
	
	delete c;
	numChars--;
	
	switch (type) 
	{
		case CHAR_MC: enemyCount--; break;
		case CHAR_NPC: npcCount--; break;	
		case CHAR_PC: playerCount--; break;
	}
}

void CharController::removeType(int type)//remove all characters of this type
{
	Character* c;
	Character* deleteChar;
	for(c = head; c != NULL; )//the update mechanism is placed in the loop
	{
		if(c->type == type)
		{
			deleteChar = c;
			c = c->next;
			remove(deleteChar->serialNo);
		}
		else
			c = c->next;//only advance if nothing was removed
	}
}

void CharController::traverse(void(*function)(Character* c, void* ptr), void* ptr)
{
	//for now it only traverses from head to tail
	Character* curr;
	for(curr = head; curr != NULL; curr = curr->next)
	{
		//process node in list
		function(curr, ptr);
	}
	
}

Character* CharController::iterate(int type, bool forward)
{
	//iterate means we return the current node pointed to by the iterator and advance the iterator accordingly
	if(iterator == NULL)
		return NULL;
	
	while (type > -1 && iterator != NULL && iterator->type != type)
	{
		iterator = (forward) ? iterator->next : iterator->prev;
	}
	
	Character* retNode = iterator;//could potentially be null, so watch out
	/*
	 advance iterator by one if possible
	 */
	if(iterator != NULL)
		iterator = (forward) ? iterator->next : iterator->prev;
	
	return retNode;	
}


void CharController::save(const char* fileName)
{
	FILE* fout;
	fout = fopen(fileName, "wb");
	
	//save all pc chars for now
	for (Character* c = head; c != NULL; c = c->next) 
	{
		if(c->type == CHAR_PC)
		{
			fwrite(&c->defIndex, 1, sizeof(int), fout);
			fwrite(&c->serialNo, 1, sizeof(int), fout);
			fwrite(&c->type, 1, sizeof(int), fout);
			fwrite(&c->currHp, 1, sizeof(int), fout);
			fwrite(&c->xPos, 1, sizeof(int), fout);
			fwrite(&c->yPos, 1, sizeof(int), fout);
			fwrite(&c->def, 1, sizeof(CharDef), fout);
			
			
			char itemsFileName[32];
			sprintf(itemsFileName, "ingame/ICS%u%s", c->serialNo, fileName);
			c->charICS->save(itemsFileName);
		}
	}
	
	fclose(fout);
}

void CharController::load(const char *fileName)
{
	FILE* fin;
	fin = fopen(fileName, "rb");
	
	//as each char is loaded add it to linked list

	int ch;
	while ((ch = fgetc(fin)) != EOF) //test if eof is reached
	{
		ungetc(ch, fin); //eof was not reached so unget the last character

		//retrieve character data
		int defIndex;
		int serialNo;
		int type;
		int currHp;
		int xPos;
		int yPos;
		
		fread(&defIndex, 1, sizeof(int), fin);
		fread(&serialNo, 1, sizeof(int), fin);
		fread(&type, 1, sizeof(int), fin);
		fread(&currHp, 1, sizeof(int), fin);
		fread(&xPos, 1, sizeof(int), fin);
		fread(&yPos, 1, sizeof(int), fin);
		
		push_back(serialNo, defIndex, type, yPos, xPos);
		Character* c = getCharacter(serialNo);
		c->currHp = currHp;

		//retrieve def data
		fread(&c->def, 1, sizeof(CharDef), fin);
		
		char itemsFileName[32];
		sprintf(itemsFileName, "ingame/ICS%u%s", c->serialNo, fileName);
		c->charICS->load(itemsFileName);
	}
	
	fclose(fin);
}

bool CharController::attemptStep(Character* c, int direction, int steps)
{
	int y,x;//the destination spot
	getPositionFrom(c->yPos, c->xPos, direction, steps, y, x);
	
	if(step(c, y, x, direction, steps))
	{
		completeStep(c, direction, steps);
		return true;
	}
	
	return false;
}

//return value is if step occurred and nothing unusual happened
bool CharController::step(Character* c, int y, int x, int direction, int steps)//new coordinates of moved character
{
	//check if step is possible
	/*
	 Ultimately, a step is only possible if the bytemap contains a floor tile underneath
	 even then, the spot will be checked for possible blocking rects
	 */
	bool goodStep = false;
	char floorTile = app->map.getByte(y, x); 
	
	bool strong = (c->type == CHAR_PC); //only players can push blocks
	switch (floorTile) 
	{
		case FLOOR_TILE: 	
			//traverse through all actionrects to see if stepping here is possible			
			if(app->map.getRects()->isSpotBlocked(y, x, direction, strong))
				break; //cannot step

			//make sure that no npcs are standing there
			if(getCharacter(y, x) != -1)
				break; //cannot step on an npcs

			//check if step is attempted on movable block that can be moved
			goodStep = checkMovableObject(y, x, direction);
			break;
	
		case -1: //character went off screen but we will allow this
			goodStep = true;
			break;
	
	//default nothing is done and no step is taken
	}
	
	return goodStep;
}



bool CharController::checkMovableObject(int y, int x, int direction)
{
	int id;
	int startY, startX;
	int height, width;
	int length;
	ActionRectList* arl;
	//an obstacle may be larger than 1x1 so it's entire backside must be checked
	//get id of object so we can get the starting point to check
	arl = app->map.getRects();
	id = arl->getObjId(OBSTACLE, y, x);//get only the obstacle at this point if one exists
	if(id < 0)
		return true;//no obstacle was in the way
		
	arl->getDimensions(id, &startY, &startX, &height, &width);
	
	bool orientation;
	switch(direction)
	{
		case DIR_UP: 
			startY--; 
			length = width; 
			orientation = HORIZONTAL; break;
		case DIR_DOWN: 
			startY += height; 
			length = width; 
			orientation = HORIZONTAL; break;
		case DIR_LEFT: 
			startX--; 
			length = height; 
			orientation = VERTICAL;	break;
		case DIR_RIGHT: 
			startX += width; 
			length = height; 
			orientation = VERTICAL; break;
	}
	
	//works for now, but the row should be checked for any blockable rects on top (and this time obstacles will block)
	int y1, x1;
	y1 = x1 = 0;
	
	bool result = true;
	char floorTile;
	for (int i = 0; i < length; i++) 
	{
		floorTile = app->map.getByte(startY + y1, startX + x1);
		
		if(floorTile != FLOOR_TILE) //can't push an obstacle onto a non-floor tile
			return false;

		result = arl->isSpotBlocked(startY + y1, startX + x1, direction, false);//strength is false for blocks
		
		if(result == true)//if a spot was blocked return false because the block can't be moved
			return false;
		
		if(orientation == HORIZONTAL)
			x1++;
		else
			y1++;
	}
	
	return true;
}

void CharController::getPositionFrom(int startRow, int startCol, 
									int direction, int steps, 
									int& endRow, int& endCol)
{
    switch (direction) {
        case DIR_UP: 
            endCol = startCol;
            endRow = startRow - steps; break;
        case DIR_DOWN: 
            endCol = startCol;
            endRow = startRow + steps; break;
        case DIR_RIGHT: 
            endCol = startCol + steps;
            endRow = startRow; break;
        case DIR_LEFT: 
            endCol = startCol - steps;
            endRow = startRow; break;
    }
}

int CharController::pushInto(Character *c, int y, int x)
{
	//get new coordinates(this should be it's own function	
	char floorTile = app->map.getByte(y, x); 
	//char occupier = mvwinch(screen->mainWin, y, x);
	
	/*
	 could push into a:
	 sign !
	 healing spring A
	 obstacle 
	 secret passage
	 npc @
	 */
	
	switch (floorTile) //only push into objects on the floor
	{
		case FLOOR_TILE:
			//there should only be one rect on the spot adjacent to us
			//so just operate on that
			return app->map.getRects()->getObjId(y, x);//return the id of what is adjacent
			break;
				
		case 'A': //a healing spot (may be used for debug only)
			c->currHp = c->def.maxHp;
			app->printSimpleHeader("HP restored!");
			break;
			
		//pushing into a solid boundary
	}
	
	return -1;
}

void CharController::completeStep(Character* c, int direction, int steps)//destination tile has already been checked
{
	int newY = c->yPos;//initialize to current values
	int newX = c->xPos;
	switch (direction) {
		case DIR_UP: newY -= steps; break;
		case DIR_DOWN: newY += steps; break;
		case DIR_RIGHT: newX += steps; break;
		case DIR_LEFT: newX -= steps; break;
	}
	
	move(c->serialNo, newY, newX);
	stepCount += steps;
}

void CharController::moveBack(Character* c)//move char back to previous position
{
	move(c->serialNo, c->prevY, c->prevX);
}

void CharController::move(int serNo, int y, int x)//see header file for more info 
{
	Character* c = getCharacter(serNo);
	c->prevX = c->xPos; //save old X
	c->xPos = x;
	c->prevY = c->yPos; //save old Y
	c->yPos = y;
}

void CharController::moveToEdge(Character* c, int edgeDirection)
{
	//Character* c = getCharacter(serNo);
	
	switch (edgeDirection) {
		case DIR_LEFT:
			c->prevX = -1; //clear out old X
			c->xPos = 0; break;
		case DIR_RIGHT:
			c->prevX = -1; //clear out old X
			c->xPos = MAX_COLS - 1; break;
		case DIR_UP:
			c->prevY = -1; //clear out old Y
			c->yPos = 0; break;
		case DIR_DOWN:
			c->prevY = -1; //clear out old Y
			c->yPos = MAX_ROWS - 1; break;
	}
}

void CharController::moveToOppositeEdge(Character* c, int currEdgeDir)
{
	//i could've used moveToEdge in the case statements, but for the sake of run-time efficiency i didn't
	switch (currEdgeDir) {
		case DIR_LEFT:
			c->prevX = -1; //clear out old X
			c->xPos = MAX_COLS - 1; break;
		case DIR_RIGHT:
			c->prevX = -1; //clear out old X
			c->xPos = 0; break;
		case DIR_UP:
			c->prevY = -1; //clear out old Y
			c->yPos = MAX_ROWS - 1; break;
		case DIR_DOWN:
			c->prevY = -1; //clear out old Y
			c->yPos = 0; break;
	}
}

int CharController::checkFloor(Character* c)
{
	//get character position
	int x,y;
	x = c->xPos;
	y = c->yPos;
	
	//check maps ics to see if item is at that spot
//	MapItem* mi = app->map.getItems()->getItem(x, y);
	int index = app->mapItems.getItemIndex(y, x);
	ICSitem* item = app->mapItems.getItemAt(index);

	if(item != NULL) //item was found
	{
		int ref = item->ref;
		c->charICS->add(ref, item->count); //add item to character inventory
//		app->map.getItems()->remove(mi);		//remove item from map
		app->mapItems.removeAt(index);

		return ref;
	}
	//else theres no item
	return -1;
}

bool CharController::isCharOnMap(Character* c)
{
	return ((c->xPos >= 0 && c->xPos < MAX_COLS) && (c->yPos >= 0 && c->yPos < MAX_ROWS));
}

int CharController::getCharacter(int y, int x) //find character at coordinates
{
	int result = -1;
	
	for(Character* c = head; c != NULL; c = c->next)
	{
		if(c->xPos == x && c->yPos == y)
		{
			result = c->serialNo;
			break;
		}
	}
	return result;
}

void CharController::renderChars() //updates positions of characters in window
{
	Character* c;
	for(c = head; c != NULL; c = c->next)
	{
		renderChar(c);
    }
  
}

void CharController::renderChar(Character* c) //updates positions of single character
{
	if(c == NULL)//make sure character isn't null
		return;
	
	//for now render only main pc and npcs, no monsters drawn on maps
	if(c->type == CHAR_NPC)
	{
		//mvwaddch(screen->mainWin, c->yPos, c->xPos, (char)c->def.symbol);//draw character on top of map
		mvwaddch(screen->mainWin.data, c->yPos, c->xPos, (char)c->def.symbol);//draw character on top of map
	}
	else if(c->type == CHAR_PC)
	{
		if(c->serialNo == app->mainPCsn)
		{
			mvwaddch(screen->mainWin.data, c->yPos, c->xPos, (char)c->def.symbol);//draw character on top of map
		}
	}
}

Character* CharController::getCharacter(int serialNo)//eventually should require an argument
{
	//cycle through linked list until character is found
	//again I think this is odd that I'm returning the node in the linked list and not just the data
	
	Character* c;
	
	for(c = head; c != NULL; c = c->next)
	{
		if(c->serialNo == serialNo)
		{
			return c;//character was found so return a pointer to it
		}
	}

    return NULL;//will need to be able to return more than just the main character
}

int CharController::getTypeCount(int type, int status, bool afflicted)
{
	assert(type == CHAR_PC || type == CHAR_NPC || type == CHAR_MC);
	
	int count = 0;
	
	if (status == -1) //no status search condition
	{
		switch (type) 
		{
			case CHAR_MC: count = enemyCount; break;
			case CHAR_NPC: count = npcCount; break;	
			case CHAR_PC: count = playerCount; break;
		}
	}
	else //positive status means find all characters of that status
		//negative status means find all characters without that status
	{
		Character* c;

		int bit;
		int increment;
		for(c = head; c != NULL; c = c->next)
		{
			if(c->type == type)
			{
				bit = CheckBit(c->def.status, status);//use positive value of status
				if(bit)//character is afflicted with status
				{
					if(afflicted)//we were looking for characters afflicted with this status
						count++;
				}
				else //character is not afflicted with status
				{
					if(!afflicted)//we were looking for characters not afflicted with this status
						count++;
				}
			}
		}
	}
		
	return count;
}

void CharController::getTypeSerialNos(int type, int nums[])
{
	Character* c;
	int ndx = 0;
	for(c = head; c != NULL; c = c->next)
	{
		if(c->type == type)
		{
			nums[ndx++] = c->serialNo;
		}
	}
}

void CharController::displayPostText(int abilRef, Character* user, Character* target, int targetSide, int targetSet)
{
	char actionMsg[32];
	switch(targetSet)
	{
		case SINGLE: sprintf(actionMsg, MAL[abilRef].posttext, target->def.name); break;
		case SIDE: 
		{		
			char side[10];
			if(user->type == CHAR_PC)
			{
				if(targetSide == SELF_SIDE)
					strcpy(side, "Players");
				else if(targetSide == OPP_SIDE)
					strcpy(side, "Enemies");
			}
			else if(user->type == CHAR_MC)
			{
				if(targetSide == SELF_SIDE)
					strcpy(side, "Enemies");
				else if(targetSide == OPP_SIDE)
					strcpy(side, "Players");
			}
			
			sprintf(actionMsg, MAL[abilRef].posttext, side);
		} break;
	}
	
	app->printPausingHeader(actionMsg);
}


bool CharController::processFightAction(int abilRef, Character* user, Character* target, int targetSet, int targetSide)
{	
	char actionMsg[32];
	int effectResult;
	bool endfight = false;
	//the targetset and targetside parameters are not necessarily the same as what is stored in effect
	// they are what the player or enemy has chosen for the ability
	
	//display pretext
	if(MAL[abilRef].pretext[0] != 0)
	{
		sprintf(actionMsg, MAL[abilRef].pretext, user->def.name);
		app->printPausingHeader(actionMsg);
	}
	//process effect
	effectResult = processActionEffect(MAL[abilRef].effect, user, target, targetSet, targetSide);

	wrefresh(screen->mainWin.data); //to reflect the changes from applying effects
					
	if(effectResult != FAILED) //if effect did not fail(not the same as straight up success)
	{
		//display posttext(the editor doesn't allow blank string entries so it will start with a ? if blank)
		//this might not be a good temporary fix
		
		if (MAL[abilRef].posttext[0] != '?')
		{
			displayPostText(abilRef, user, target, targetSide, targetSet);//involves more work than pretext
		}
		
		if(effectResult == ESCAPE_FIGHT)
			endfight = true;
	}
	else
		app->printPausingHeader("Technique failed.");
	
	return endfight;
}

int CharController::processActionEffect(Effect& effect, Character* user, Character* target, int targetSet, int targetSide)
{
	logHandle.logData(effect.type);
	static int result;//this is made static only so suck works
	Character* c;
	int targetType;
	float multiplier = effect.multiplier;

	int aliveCount;
	int oppType;
	int returnCode = SUCCESS;

	switch (effect.type) 
	{
		case ALTER_HP:
			//this target set stuff maybe should be done before we get to the alter hp case?
			switch(targetSet)//if target is a single character
			{
				case SINGLE:
					if (isDead(target) == false) //only use technique on living character
					{
						result = processEffectBasis(effect.basis, multiplier, user, target);
						app->printHPChange(target->serialNo, result);
					}
					else
						returnCode = FAILED;
					break;
				case SIDE:
					oppType = (user->type == CHAR_PC) ? CHAR_MC : CHAR_PC;
					targetType = (targetSide == OPP_SIDE) ? oppType : user->type;

					aliveCount = getTypeCount(targetType, ALIVE);
					multiplier /= (float)aliveCount; 

					//even on a side, effects are applied one character at a time
					for (c = head; c != NULL; c = c->next) 
					{
						if(c->type == targetType && isDead(c) == false)
						{
							result = processEffectBasis(effect.basis, multiplier, user, c);
							app->printHPChange(c->serialNo, result);
						}
					}
					break;
			}
			break;
		case ALTER_STAT:
			switch (targetSet) {
				case SINGLE:
					//TODO - there may be factors that influence what works and what doesn't
					
					//apply stat only if target is alive, or if target is dead, and the effect is to revive
					bool reviving = (CheckBit(effect.status, ALIVE));
					if (isDead(target) == false || reviving) 
					{
						int statCapture = target->def.status;//what status is like before effects are applied
						target->def.status |= effect.status;
						
						if (statCapture != target->def.status) //a change took place
						{
							//if basis is not 0, then it may change hp
							result = processEffectBasis(effect.basis, multiplier, user, target);
							if(result > 0)
								app->printHPChange(target->serialNo, result);
							
							if(reviving)//if character was revived then they get their turn back
							{
								app->turnTracker.enablePlayer(target->serialNo, true);
							}
						}
						else
							returnCode = FAILED;
					}
					else
						returnCode = FAILED;
					
					break;
			}
			
			break;
		case ALTER_FIGHT:
			returnCode = ESCAPE_FIGHT;
			break;
	}
	return returnCode;
}

int CharController::processEffectBasis(int basis, float multiplier, Character* user, Character* target)
{
	int weaponDamage;
	int value = -1;
	
	switch (basis) 
	{
		case PHYSICAL:
			weaponDamage = (user->def.weapon >= 0) ? MIL[user->def.weapon].value : 1;
			
			//calculate how much damage is incurred
			value = (int)((user->def.strength * weaponDamage) * multiplier);
			
			target->currHp += value; //alter target hp(we always add; multiplier may be negative)
			if(target->currHp <= 0) //hp cannot go below 0
			{
				//target has died
				target->currHp = 0;
				ClearBit(target->def.status, ALIVE);//set status to dead 
				app->turnTracker.enablePlayer(target->serialNo, false);//take away their turn
			}
			else if(target->currHp > target->def.maxHp)
				target->currHp = target->def.maxHp;
			
		break;
	}
	return value;
}

bool CharController::processAIRoutine(Character* c, int& abilRef, int& targetId)
{	
	list<AIStep> steps;

	//TODO - figure out a better way to read in the appropriate routine for the character
	AIStep ais1 = {SELF_SIDE, {0, 1}, HP, {0,0,0,'<'}, 50, '%', 6, SELF_SIDE, {0, '^'}, 100}; //if an ally has less than 50%, heal everyone
	steps.push_back(ais1);
	
	list<AIStep>::iterator it;
	
	//TODO - allow for many more types of aisteps to be processed
	AIStep* chosenStep = NULL;
	Character* target = NULL;
	for (it = steps.begin(); it != steps.end() && chosenStep == NULL; it++) 
	{
		if(processAIStep(c, (*it), target))//this step is true and must be executed(until I decide to implement chance!)
			chosenStep = &(*it);
	}
	
	if (chosenStep == NULL) //if no step was true then use the default
		chosenStep = &defaultStep;

	//setup chosen ability and target using chosen step
	abilRef = chosenStep->response;
	
	if(chosenStep->targetSide == SELF_SIDE)
	{	
		if(chosenStep->targetSet[1] == '^')
		{
			targetId = _CHAR_MC;
		}
		else
			targetId = target->serialNo;
	}
	else//targetside == OPP_SIDE
	{
		if(chosenStep->targetSet[1] == '^')
		{
			targetId = _CHAR_PC;
		}
		else if(chosenStep->targetSet[1] == '?')//pick random player
		{
			//pick one of several targets
			int randNo;
			
			begin();
			randNo = rand() % playerCount;
			Character* recipient = NULL;
			for (int i = 0; i < playerCount; i++) 
			{
				if (i == randNo) {
					recipient = iterate(CHAR_PC);
					if(isDead(recipient))//the player randomly picked is dead, so try again
					{
						randNo = rand() % playerCount;
						begin();
						i = -1;//it will be iterated back up to 0
					}
					else//found living player
					{
						targetId = recipient->serialNo;
						break;
					}
					
				}
				else
					iterate(CHAR_PC);
			}
		}
		else
			targetId = target->serialNo;
	}

	return true;
}

bool CharController::processAIStep(Character* c, AIStep& step, Character* target)
{
	//TODO finish this
	//simplify as much left operand data as possible
	int observationType = (step.observationSide == SELF_SIDE) ? CHAR_MC : CHAR_PC;
	int count = (step.observationSide == SELF_SIDE) ? enemyCount : playerCount;
	
	//process observation set better
	int setToken = step.observationSet[1];
	int setCount;
	
	//TODO - negation, retain
	switch (setToken) 
	{
		case '?': 
			setCount = 1; //only 1 random character must be tested
			count = 1;
			break;
		case '^': //all on a side must meet the condition
			setCount = count;
			break;
		case '@': //only the turnholder must be checked
			count = 1;
			setCount = 1;
			break;
		default://should be a number, at least setToken characters must meet condition
			setCount = setToken;
			break;
	}
	
	
	//setup the correct conditional function
	bool (*conditional)(int, int) = NULL;
	
	switch (step.relOp[3]) 
	{
		case '<': conditional = conditional_lt; break;
		case '>': conditional = conditional_gt; break;
		case '=':
		{
			switch (step.relOp[2]) 
			{
				case '=': conditional = conditional_eq; break;
				case '<': conditional = conditional_leq; break;
				case '>': conditional = conditional_geq; break;
				case '!': conditional = conditional_neq; break;
			}
		}	
			break;
			
		default: count = 0; break;
	}
	
	//begin iterations
	begin();
	int leftOp, rightOp;
	int trueCount = 0;
	for (int i = 0; i < count; i++) 
	{
		//use appropriate target choosing pattern
		switch (setToken) {
			case '?':
				target = getRandomCharacter(observationType, ALIVE);
				break;
			case '@':
				target = c;
				break;
			default:
				target = iterate(observationType);//find the next living target(may need to rework this for revive to work)
				break;
		}
		
		if(isDead(target)) {
			continue;//advance to next target which hopefully is alive
		}
		
		//determine the left and right operands
		if (step.leftId == HP) 
		{
			leftOp = target->currHp;
			
			if (step.rightId == '%') {
				rightOp = (int)(target->def.maxHp * ((double)(step.rightOperand) / 100.0)) + 0.5;
			}
		}
		
		if(conditional(leftOp, rightOp)) //if true
			trueCount++;
		
		if (trueCount >= setCount) {
			break;
		}
	}
	
	//check if entire step is true
	return trueCount >= setCount;
}


//TODO - maybe rethink whether we should be able to select a random character with a particular status ailment
Character* CharController::getRandomCharacter(int type, int status, bool afflicted)
{
	//pick one of several targets
	int randNo;
	Character* randChar = NULL;
	
	begin();
	int count = getTypeCount(type);//get all of the type here
	int subCount = getTypeCount(type, status, afflicted);
	
	if(count == 0 || subCount == 0)//no characters meet the search criteria
		return randChar;//should be null here
	
	randNo = rand() % count;
	
	//search through 
	for (int i = 0; i < count; i++) 
	{
		if (i == randNo) 
		{
			randChar = iterate(type);
			
			if(status == -1)
				break;
			
			if(CheckBit(randChar->def.status, status))//character is afflicted with status
			{
				if(afflicted)//we were looking for characters afflicted with this status
					break;
			}
			else //character is not afflicted with status
			{
				if(!afflicted)//we were looking for characters not afflicted with this status
					break;
			}
			
			//must check different one
			randNo = rand() % count;
			begin();
			i = -1;//it will be iterated back up to 0			
		}
		else
			iterate(type);
	}
	
	return randChar;
}

bool CharController::setType(int id, int newType)
{
	Character* c = getCharacter(id);
	
	int oldType = c->type;
	switch (oldType) {
		case CHAR_PC: playerCount--; break;
		case CHAR_NPC: npcCount--; break;
		case CHAR_MC: enemyCount--; break;			
		default:
			break;
	}
	
	c->type = newType;
	
	switch (newType) {
		case CHAR_PC: playerCount++; break;
		case CHAR_NPC: npcCount++; break;
		case CHAR_MC: enemyCount++; break;	
	}
	
	assert(playerCount + enemyCount + npcCount == numChars);
}

void CharController::printStats(Character* c, WINDOW* w, bool fullStats)
{
	//clear the window
	werase(w);//I may not want to do this if multiple data sets are printed in the window
	
	//add character data here
	char statusData[100];
	
	/*
	 The below sprintf statements only work if the data is flush with the left edge of the window
	 this may need to be fixed later
	 
	 also I may want to make brief stats first and then add full stats to it if fullStats is true
	 */
	if(fullStats)//prints everything
	{
		char weaponStr[16];
		getWeaponName(c, weaponStr);
				
		int nextLevel = getLevelData(c->def.level, "EXP") - c->def.exp;
		if(nextLevel < 0)
			nextLevel = 0;
		
		sprintf(statusData, "%s\nHP: %u/%u\nLevel %u\nExp %u\nTo Next %u\nStrength %u\nWeapon %s", 
				c->def.name, c->currHp, c->def.maxHp, 
				c->def.level, c->def.exp, 
				
				nextLevel,//exp to reach next level
				
				c->def.strength,
				weaponStr
				);
	}
	else//prints only brief status (for fighting and such)
	{
		sprintf(statusData, "%s\nHP: %u/%u\nLevel %u\nExp %u", 
			c->def.name, c->currHp, c->def.maxHp, 
			c->def.level, c->def.exp);
	}
	
	waddstr(w, statusData);
	wrefresh(w);
}

void CharController::getWeaponName(Character* c, char* buffer)
{
	if(buffer == NULL)
		return;
	
	//char weaponStr[16];
	if(c->def.weapon < 0)
	{
		strcpy(buffer, "Nothing");
	}
	else
		strcpy(buffer, MIL[c->def.weapon].name);
	

}

void CharController::equip(Character* c, int milRef, int type, bool equipOn)
{	
	if(MIL[milRef].category != type)//don't equip something that isn't a weapon
		return;
	
	//always unequip to start with, then equip if equipOn is enabled
	c->def.weapon = -1;//unequip previous weapon first
	
	if(equipOn)
		c->def.weapon = milRef;
}

void CharController::victory(int partyId)
{	
	//display victory
	app->printPausingHeader("Victory!");
	
	//collect spoils
	char victoryMsg[32];
	
	int totalExp = 0;
	
	for (Character* c = head; c != NULL; c = c->next) 
	{
		if(c->type == CHAR_MC)
		{
			totalExp += c->def.exp;
		}
	}
	
	sprintf(victoryMsg, "Gained %u exp.", totalExp);
	app->printPausingHeader(victoryMsg);
	
	//this is sort of cheap, using the gameapps storage facility instead of writing a new one for the cc class!
	app->storage[0] = totalExp; 
	traverse(applyExp, this);
}

void CharController::applyExp(Character* c, void* ptr)
{
	CharController* cc = (CharController*) ptr;
	
	//only living players gain experience
	if (c->type != CHAR_PC || cc->isDead(c))
		return;
	
	c->def.exp += cc->app->storage[0];
	if(c->def.exp > MAX_EXP)//do not exceed maximum amount of experience
		c->def.exp = MAX_EXP;
	
	//should check for level up here as well
	int levelsGained = cc->checkLevelUp(c);
	
	//print celebratory message for level ups
	if(levelsGained > 0)
	{
		char gainLevelMsg[48];
		if(levelsGained == 1)
			sprintf(gainLevelMsg, "%s gained a level.", c->def.name);
		else
			sprintf(gainLevelMsg, "%s gained %u levels.", c->def.name, levelsGained);
		cc->app->printPausingHeader(gainLevelMsg);
	}
}

int CharController::checkLevelUp(Character *player)
{
	bool checkingLevels = true;
	
	/*we use a loop here because a player could gain multiple levels from a single battle*/
	int levelsGained = 0;
	while (checkingLevels) 
	{
		//determine how much exp would be needed to gain a level
		long expNeeded = getLevelData(player->def.level, "EXP");
		
		//compare expNeeded to players experience
		if(expNeeded > 0 && expNeeded <= player->def.exp)
		{
			//a level was gained
			levelsGained++;
			player->def.level++;
			
			long newHp = getLevelData(player->def.level, "HP");
			long hpRaise = newHp - player->def.maxHp;
			
			player->def.maxHp = (int)newHp;//update max hp
			player->currHp += (int)hpRaise;//give curr hp bonus
			if(player->currHp > player->def.maxHp)//current hp should never surpass max hp
				player->currHp = player->def.maxHp; 
			
			player->def.strength = (int)getLevelData(player->def.level, "STRENGTH");
			//update other stats as well here
		}
		else
			checkingLevels = false;
	}
	
	return levelsGained;
}

long CharController::getLevelData(int level, const char* type)
{
	if (level <= 0) {//error checking
		return -1;
	}
	
	if(levelDataReader.is_open() == false)//file to chart should be open
		return -1;
	
	//all lines should be the same size
	char line [256];
	char header[colWidth];

	levelDataReader.seekg(0, ios::beg);//always reset file to beginning
	levelDataReader.getline(line, 256);
	
	stringstream ss(line);
	int colNdx = 0;
	while(ss >> header)
	{
		if(strcmp(header, type) == 0)
		{
			break;
		}
		else
			colNdx++;//check next column(problem, what if we request a col that doesn't exist?)
	}
	
	
	long lineSize = (long)strlen(line) + 1; //add 1 because the line does not include the newline character

	long lineOffset = (level * lineSize);
	levelDataReader.seekg(lineOffset + (colNdx * colWidth), ios::beg);
	
	long data = -1;
	
	levelDataReader >> data;
	
	return data;
}


/*
unsigned int CharController::computeLevelUp(int level)
{
	
	 after using open office i approximated a trendline to the level chart for ff9.
	 the format is a*x^b
	 the values I used were a = 0.5, b = 3.57
	 
	
	//should this be hard coded, 
	//loaded from a file, or 
	//generated by an algorithm?
	
	unsigned int levels_exp[MAX_LEVELS + 1]; //stores the amount of experience needed to level up
		//(we add 1 because we don't need 0 index)
	
	levels_exp[0] = 0;//characters always start on at least level 1
	int expNeeded;
	
	
	//set up data to form a cubic equation(this gives us the curve we want)
	//a cubic equation gradually gets larger and larger, and then plateaus
	//we would want the player to have to get more and more experience to level up,
		// but towards the end of the game or when the player has earned an obscene number of levels
		// there's no point in have to fight so many battles to gain levels
	
	double a, b, c, d;//terms in the polynomial
	unsigned int sum; //sum of terms


	double cubicCoef, quadCoef, linearCoef; //coefficients
	
	 meanings of coefficients
	 d = just moves the graph up and down
	 linearCoef: the more positive it is the closer it brings the graph to a straight line
		the more negative the more undulating it is
		if used at all it should be mildly positive (about 1)
	 
	 quadCoef: moves the graph left and right and produces lesser/greater variability in the inflection
		if you want a horizontal/vertical tangent in the middle of the graph this should be set close to 0
	 
	 cubicCoef: widens the 2 legs of the graph, the point where the inflection changes never moves
		if set to 0 the equation turns into a quadratic equation
		this should probably be less than 1 or else the graph will be very narrow
	 
	
	d = 0;//these were values I chose long ago to get the right curve
	cubicCoef = 1;
	quadCoef = 0.0;
	linearCoef = 1;
	int x;
	int final;
	fprintf(fp, "Level data:\n");
	for(int i = 1; i < MAX_LEVELS; i++) //cubic is computed for as many levels as their are(about)
	{	
		
		a = cubicCoef * (pow((double)x, 3));
		b = quadCoef * (pow((double)x, 2));
		c = linearCoef * x;
		//d is already computed!
		
		//execute cubic equation
		fprintf(fp, "%u) ", i);//level
		sum = (int)(a + b + c + d);//sum terms in cubic equation(I don't care if the mantissa is thrown out)
		
		//transform data so that the curve is what we want
		sum = -sum; //reflect sum about x axis
		
		
		
		
		
	//	fprintf(fp, "%u ", levels_exp[i]);//amount computed
	//	levels_exp[i] = levels_exp[i] + levels_exp[i - 1];

	//	fprintf(fp, "%u\n", levels_exp[i]);//amount needed to level up
		
	}	
	
}*/

void CharController::death()
{
	//display death message
	app->printPausingHeader("You died.");
}

bool CharController::isDead(Character* c)
{
	return !CheckBit(c->def.status, ALIVE); //if alive is set then return
}


bool CharController::isImmobilized(Character* c)
{
	//check all conditions that would indicate immobilization
	//for now that only means death
	if(isDead(c))
		return true;
	
	//add other immobilzation condition here
	
	return false;
}

bool CharController::isTypeImmobilized(int type)
{
	int count = (type == CHAR_PC) ? playerCount : enemyCount;
	
	Character* c;
	begin();
	for (int i = 0; i < count; i++)
	{
		c = iterate(type);
		if(isImmobilized(c) == false) //if just 1 of a certain type is false then the type is not immobile
			return false;
	}
	return true;
}

bool CharController::areEnemiesCheckmated()
{
	Character* c;
	for(c = head; c != NULL; c = c->next)
	{
		if(c->type == CHAR_MC)
		{
			if (!isDead(c)) //if not dead
			{
				return false;
			}
		}
	}
	
	return true;
}

void CharController::free()
{
	MAL = NULL;
	screen = NULL;
	mHead.clear();
	delete head; //this should end up deleting the tail as well
	numChars = 0;
	levelDataReader.close();
}







