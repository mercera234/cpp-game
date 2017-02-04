#pragma once

enum {
    INIT_PURPOSE = 0,
    RUN_PURPOSE,
    SHUTDOWN_PURPOSE
};
//a linked list of pointers to functions
//  the function takes a pointer and a purpose
typedef struct state 
{
    void (*function)(void *ptr, int purpose);
    state* next;
    
    state()
    {
        function = NULL;
        next = NULL;
    }
    
    ~state()
    {
        delete next;
    }
}state;

typedef void(*func)(void*, int);

//a stack of states
class StateManager
{
private:
    state* topState;
	bool stackAltered = false;

public:
    StateManager();
    ~StateManager();
    
	func getTop(){ return topState->function;}

	//next three methods return true if stack is successfully altered
    bool push(void (*function)(void* ptr, int purpose), void* dataPtr = NULL);
    bool pop(void *dataPtr = NULL);
    bool popAll(void *dataPtr = NULL);
    bool process(void *dataPtr = NULL);
	bool isAltered();
	void setStateAltered(bool);
};

