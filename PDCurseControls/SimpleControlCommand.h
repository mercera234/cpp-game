#pragma once
#include "Controllable.h"

template <class Receiver>
class SimpleControlCommand : public ControlCommand
{
protected:
	typedef int (Receiver::* Action)(Controllable*, int);

	Receiver* receiver;
	Action action;
public:
	SimpleControlCommand() {}
	~SimpleControlCommand() {}

	virtual int execute(Controllable*, int);

	void setReceiver(Receiver* r);
	void setAction(Action a);
};


template <class Receiver>
void SimpleControlCommand<Receiver>::setReceiver(Receiver* r)
{
	receiver = r;
}

template <class Receiver>
void SimpleControlCommand<Receiver>::setAction(Action a)
{
	action = a;
}


template <class Receiver>
int SimpleControlCommand<Receiver>::execute(Controllable* c, int input)
{
	return (receiver->*action)(c, input);
}
