//
//  stateManager.cpp
//  basicgame
//
//  Created by Alex Mercer on 11/10/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include <iostream>

#include "stateManager.h"

StateManager::StateManager()
{
    topState = NULL;
}

StateManager::~StateManager()
{
    popAll(); //delete entire stack
}

bool StateManager::pop(void *dataPtr)
{
    state* curr;
    
    if(topState == NULL)//if stack is empty return false
    {
        return false;
    }
    else //there are more states to pop
    {
        curr = topState; //save allocated memory to curr
        topState->function(dataPtr, SHUTDOWN_PURPOSE); //shutdown the top
        topState = curr->next; //move top down to next stack item
        curr->next = NULL; //delete memory pointed to by curr
        delete curr;
    }

	stackAltered = true;
    return true;
}

bool StateManager::popAll(void *dataPtr)
{
    while (pop(dataPtr) == true);
	stackAltered = true;
	return true;
}

//push state onto stack; pushing also runs the function
bool StateManager::push(void (*function)(void* ptr, int purpose), void* dataPtr)
{
    if(function == NULL) //make sure function is valid
        return false;
    
    state* curr = new state;
    curr->function = function;
    curr->next = topState;
    topState = curr;
    
    curr->function(dataPtr, INIT_PURPOSE);

	stackAltered = true;
	return true;
}

//process function previously pushed onto stack
bool StateManager::process(void *dataPtr)
{
	stackAltered = false;
    if(topState == NULL) //make sure there is a state to process
	{
	//	logHandle.logData("topState is NULL");
		return false;
    }
    topState->function(dataPtr, RUN_PURPOSE);
    return true;
}

bool StateManager::isAltered()
{
	return stackAltered;
}


void StateManager::setStateAltered(bool stackAltered)
{
	this->stackAltered = stackAltered;
}









