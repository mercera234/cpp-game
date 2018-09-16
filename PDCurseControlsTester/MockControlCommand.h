#pragma once
#include "Controllable.h"

template <class Receiver>
class MockControlCommand : public ControlCommand
{
private:
	typedef int (Receiver::* Action)();

	Receiver* receiver;
	Action action;
public:
	MockControlCommand() {}
	~MockControlCommand() {}

	virtual int execute(Controllable*, int);

	void setReceiver(Receiver* r);
	void setAction(Action a);
};


template <class Receiver>
void MockControlCommand<Receiver>::setReceiver(Receiver* r)
{
	receiver = r;
}

template <class Receiver>
void MockControlCommand<Receiver>::setAction(Action a)
{
	action = a;
}


template <class Receiver>
int MockControlCommand<Receiver>::execute(Controllable* c, int input)
{
	return (receiver->*action)(c, input);
}
